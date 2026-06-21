#!/usr/bin/env python3
"""
MAVLink HIL <-> Gazebo Bridge — 5 sensori
  IMU        → HIL_SENSOR  (accel + gyro)
  Magnetometro → HIL_SENSOR (mag XYZ)
  Barometro  → HIL_SENSOR  (pressure, alt, temp)
  GPS        → HIL_GPS
  LiDAR      → DISTANCE_SENSOR
  Gazebo     ← HIL_ACTUATOR_CONTROLS (motor_speed)
"""

import time, math, threading
from pymavlink import mavutil

try:
    from gz.transport13 import Node
    from gz.msgs10.imu_pb2        import IMU
    from gz.msgs10.navsat_pb2     import NavSat
    from gz.msgs10.laserscan_pb2  import LaserScan
    from gz.msgs10.fluid_pressure_pb2 import FluidPressure
    from gz.msgs10.magnetometer_pb2   import Magnetometer
    from gz.msgs10.actuators_pb2      import Actuators
    GZ = True
except ImportError:
    print("[WARN] gz.transport non trovato — usa ROS 2 bridge")
    GZ = False

# ── Costanti fisiche ────────────────────────────────────────────
SEA_LEVEL_P   = 101325.0   # Pa
TEMP_K        = 288.15     # K (15 °C, ISA)
LAPSE         = 0.0065     # K/m
R             = 287.058    # J/(kg·K)
G             = 9.80665    # m/s²

def altitude_to_pressure(alt_m: float) -> float:
    """Converti quota [m] in pressione [Pa] con formula ISA."""
    return SEA_LEVEL_P * (1 - LAPSE * alt_m / TEMP_K) ** (G / (R * LAPSE))

def pressure_to_altitude(press_pa: float) -> float:
    return (TEMP_K / LAPSE) * (1 - (press_pa / SEA_LEVEL_P) ** (R * LAPSE / G))


# ── Configurazione ──────────────────────────────────────────────
MAV_ADDR   = "udpin:0.0.0.0:14560"
MODEL      = "x500"
NUM_MOTORS = 4
MAX_RADS   = 1100.0   # rad/s al 100% throttle — calibra sul tuo modello

# Topic Gazebo
WORLD = "default"
BASE  = f"/world/{WORLD}/model/{MODEL}/link/base_link/sensor"

GZ_TOPICS = {
    "imu":  f"{BASE}/imu_sensor/imu",
    "gps":  f"{BASE}/navsat_sensor/navsat",
    "baro": f"{BASE}/baro_sensor/fluid_pressure",
    "mag":  f"{BASE}/mag_sensor/magnetometer",
    "lidar":f"{BASE}/lidar_sensor/scan",
    "motors": f"/model/{MODEL}/command/motor_speed",
}


class SensorBridge:
    def __init__(self):
        # ── MAVLink ──
        self.mav = mavutil.mavlink_connection(MAV_ADDR,
                                              source_system=1,
                                              source_component=mavutil.mavlink.MAV_COMP_ID_AUTOPILOT1)
        print(f"[MAVLink] Aspetto heartbeat su {MAV_ADDR} …")
        self.mav.wait_heartbeat()
        print(f"[MAVLink] Connesso (sys={self.mav.target_system})")

        # Cache dati sensore per il fuso HIL_SENSOR
        self._imu  = None
        self._baro = None
        self._mag  = None
        self._running = True

        # ── Gazebo transport ──
        if GZ:
            self.gz = Node()
            self._motor_pub = self.gz.advertise(GZ_TOPICS["motors"], Actuators)

            self.gz.subscribe(IMU,          GZ_TOPICS["imu"],   self._on_imu)
            self.gz.subscribe(NavSat,       GZ_TOPICS["gps"],   self._on_gps)
            self.gz.subscribe(FluidPressure,GZ_TOPICS["baro"],  self._on_baro)
            self.gz.subscribe(Magnetometer, GZ_TOPICS["mag"],   self._on_mag)
            self.gz.subscribe(LaserScan,    GZ_TOPICS["lidar"], self._on_lidar)
            print("[Gazebo] Subscriptions attive.")

    # ══════════════════════════════════════════════════════════════
    #  GAZEBO → MAVLink
    # ══════════════════════════════════════════════════════════════

    def _on_imu(self, msg: IMU):
        """Aggrega IMU; HIL_SENSOR viene inviato qui fuso con baro+mag."""
        self._imu = msg
        self._send_hil_sensor()

    def _on_baro(self, msg: FluidPressure):
        self._baro = msg
        # Il baro è più lento (50 Hz) — non triggera HIL_SENSOR da solo

    def _on_mag(self, msg: Magnetometer):
        self._mag = msg

    def _send_hil_sensor(self):
        """
        Costruisce e invia HIL_SENSOR fuso da IMU + baro + mag.
        Chiamato ad ogni tick IMU (~250 Hz).
        """
        if self._imu is None:
            return

        t_us = int(time.time() * 1e6)
        imu  = self._imu

        # — Accelerometro & giroscopio —
        ax = imu.linear_acceleration.x
        ay = imu.linear_acceleration.y
        az = imu.linear_acceleration.z
        gx = imu.angular_velocity.x
        gy = imu.angular_velocity.y
        gz = imu.angular_velocity.z

        # — Magnetometro (default simulato se il plugin non è presente) —
        if self._mag is not None:
            # Gazebo pubblica in Tesla → converti in Gauss (1 T = 10000 G)
            mx = self._mag.field_tesla.x * 1e4
            my = self._mag.field_tesla.y * 1e4
            mz = self._mag.field_tesla.z * 1e4
        else:
            # Campo magnetico terrestre simulato (nord Italia ~45°N)
            mx, my, mz = 0.21, 0.01, -0.44

        # — Barometro —
        if self._baro is not None:
            abs_pressure_pa = self._baro.pressure          # Pa
            temperature     = self._baro.temperature - 273.15  # °C
        else:
            abs_pressure_pa = SEA_LEVEL_P
            temperature     = 25.0

        abs_pressure_hpa = abs_pressure_pa / 100.0
        pressure_alt     = pressure_to_altitude(abs_pressure_pa)
        diff_pressure    = 0.0   # pitot tube (non simulato)

        # Bitmask: tutti i campi validi (vedi MAVLink spec HIL_SENSOR)
        # bit 0-2: accel, 3-5: gyro, 6-8: mag, 9: abs_pressure,
        # 10: diff_pressure, 11: pressure_alt, 12: temperature
        fields_updated = 0b1111111111111

        self.mav.mav.hil_sensor_send(
            t_us,
            ax, ay, az,
            gx, gy, gz,
            mx, my, mz,
            abs_pressure_hpa,
            diff_pressure,
            pressure_alt,
            temperature,
            fields_updated,
            0           # sensor_id (0 = default)
        )

    def _on_gps(self, msg: NavSat):
        """Converte NavSat Gazebo in HIL_GPS (10 Hz)."""
        t_us = int(time.time() * 1e6)

        lat = int(msg.latitude_deg  * 1e7)   # degE7
        lon = int(msg.longitude_deg * 1e7)   # degE7
        alt = int(msg.altitude      * 1e3)   # mm (MSL)

        # Velocità NED in cm/s
        vn = int(msg.velocity_north * 100)
        ve = int(msg.velocity_east  * 100)
        vd = int(-msg.velocity_up   * 100)   # NED: down = positivo

        # Ground speed 2D e course
        gs = int(math.hypot(msg.velocity_north, msg.velocity_east) * 100)
        cog = int(math.degrees(
                  math.atan2(msg.velocity_east, msg.velocity_north)
              ) * 100) % 36000

        # EPH / EPV simulati (dipende dal plugin)
        eph = 30   # cm (0.30 m)
        epv = 50

        self.mav.mav.hil_gps_send(
            t_us,
            3,              # fix_type: 3D fix
            lat, lon, alt,
            eph, epv,
            gs,
            vn, ve, vd,
            cog,
            12              # satellites_visible
        )

    def _on_lidar(self, msg: LaserScan):
        """
        Converte LaserScan Gazebo in DISTANCE_SENSOR MAVLink.
        Usa la lettura centrale del raggio (downward-facing lidar).
        """
        if not msg.ranges:
            return

        t_ms = int(time.time() * 1e3)

        # Range centrale (o minimo, dipende dalla configurazione del sensore)
        mid    = len(msg.ranges) // 2
        dist_m = msg.ranges[mid]

        # Sanity check: filtra inf/nan
        if not math.isfinite(dist_m):
            return

        dist_cm = int(dist_m * 100)           # cm
        min_cm  = int(msg.range_min * 100)
        max_cm  = int(msg.range_max * 100)

        self.mav.mav.distance_sensor_send(
            t_ms,
            min_cm,      # min_distance [cm]
            max_cm,      # max_distance [cm]
            dist_cm,     # current_distance [cm]
            0,           # type: MAV_DISTANCE_SENSOR_LASER
            0,           # id
            25,          # orientation: MAV_SENSOR_ROTATION_PITCH_270 (downward)
            255,         # covariance (unknown)
            0.0, 0.0,    # horizontal_fov, vertical_fov (non usati)
            [0,0,0,0],   # quaternion (opzionale, ignorato se covariance=255)
            0            # signal_quality
        )

    # ══════════════════════════════════════════════════════════════
    #  MAVLink → GAZEBO  (attuatori)
    # ══════════════════════════════════════════════════════════════

    def _mavlink_rx_loop(self):
        while self._running:
            msg = self.mav.recv_match(
                type=["HIL_ACTUATOR_CONTROLS", "HEARTBEAT"],
                blocking=True, timeout=1.0)
            if msg is None:
                continue

            t = msg.get_type()

            if t == "HIL_ACTUATOR_CONTROLS":
                self._send_motor_speeds(msg)

            elif t == "HEARTBEAT":
                self.mav.mav.heartbeat_send(
                    mavutil.mavlink.MAV_TYPE_GENERIC,
                    mavutil.mavlink.MAV_AUTOPILOT_INVALID,
                    0, 0, mavutil.mavlink.MAV_STATE_ACTIVE)

    def _send_motor_speeds(self, msg):
        """
        HIL_ACTUATOR_CONTROLS.controls[0..N-1] è normalizzato [0, 1].
        Gazebo si aspetta rad/s — scala con MAX_RADS.
        """
        speeds = []
        for i in range(NUM_MOTORS):
            norm  = max(0.0, min(1.0, msg.controls[i]))
            speeds.append(norm * MAX_RADS)

        if GZ:
            act = Actuators()
            act.velocity.extend(speeds)
            self._motor_pub.publish(act)

    # ══════════════════════════════════════════════════════════════
    #  Run
    # ══════════════════════════════════════════════════════════════

    def run(self):
        print("[Bridge] In esecuzione. Ctrl+C per fermare.")
        rx = threading.Thread(target=self._mavlink_rx_loop, daemon=True)
        rx.start()
        try:
            while self._running:
                time.sleep(0.1)
        except KeyboardInterrupt:
            print("\n[Bridge] Fermato.")
            self._running = False


if __name__ == "__main__":
    SensorBridge().run()