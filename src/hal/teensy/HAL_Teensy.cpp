#include <hal/teensy/HAL_Teensy.hpp>

/*--------------------------
        HAL IMU
---------------------------*/
HAL_IMU_Teensy::HAL_IMU_Teensy(){}
HAL_IMU_Teensy::~HAL_IMU_Teensy(){}

bool HAL_IMU_Teensy::init()
{
    return imu.init();
}

void HAL_IMU_Teensy::calib()
{
    imu.calib();
}

Vector3f HAL_IMU_Teensy::readGyro() 
{
    return imu.getRawGyro();
}

Vector3f HAL_IMU_Teensy::readAccel()
{
    return imu.getRawAccel();
}

ImuData HAL_IMU_Teensy::read()
{
    return imu.getRawImu();
}

/*--------------------------
        HAL MOTOR
---------------------------*/

HAL_MOTOR_Teensy::HAL_MOTOR_Teensy(){}
HAL_MOTOR_Teensy::~HAL_MOTOR_Teensy(){}

void HAL_MOTOR_Teensy::init()
{
    motors.init();
}

void HAL_MOTOR_Teensy::test()
{
    motors.test();
}

void HAL_MOTOR_Teensy::stop()
{
    motors.stop();
}

void HAL_MOTOR_Teensy::write(float m1, float m2, float m3, float m4)
{
    motors.write((int)m1, (int)m2, (int)m3, (int)m4);
}
/*--------------------------
        HAL LIDAR
---------------------------*/
HAL_LIDAR_Teensy::HAL_LIDAR_Teensy(){}
HAL_LIDAR_Teensy::~HAL_LIDAR_Teensy(){}

bool HAL_LIDAR_Teensy::init(Stream *serialPtr)
{
    return lidar.init(serialPtr);
}

bool HAL_LIDAR_Teensy::init()
{
    return false;  // Fallback per init() senza parametri
}

void HAL_LIDAR_Teensy::calib()
{
    lidar.calib();
}

LidarData HAL_LIDAR_Teensy::read()
{
    return lidar.read();
}

/*--------------------------
        HAL GPS
---------------------------*/
HAL_GPS_Teensy::HAL_GPS_Teensy(){}
HAL_GPS_Teensy::~HAL_GPS_Teensy(){}

bool HAL_GPS_Teensy::init(Stream *serialPtr)
{
    return gps.init(serialPtr);
}

bool HAL_GPS_Teensy::init()
{
    return false;  // Fallback per init() senza parametri
}

GpsData HAL_GPS_Teensy::read()
{
    return gps.read();
}

/*--------------------------
        HAL MAG
---------------------------*/
HAL_MAG_Teensy::HAL_MAG_Teensy(){}
HAL_MAG_Teensy::~HAL_MAG_Teensy(){}

bool HAL_MAG_Teensy::init()
{
    return mag.init();
}

void HAL_MAG_Teensy::read()
{
    // TODO: Implementare lettura magnetometro
}

