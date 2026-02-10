#pragma once


#ifdef HAL_TEENSY
    #include <Arduino.h>
#endif

#include <utils/uav_data.hpp>
#include <utils/Vector3f.h>
#include <config/board_configs.h>
#include <config/parameters.h>

struct HALState{
    bool imu_state[IMU_INSTANCES];
    bool mag_state[MAG_INSTANCES];
    bool lidar_state[LIDAR_INSTANCES];
    bool gps_state[GPS_INSTANCES];
    bool baro_state[BARO_INSTANCES];
    bool motor_state;
    bool tlm_state;
    bool logger_state;

    //HALState():

};



/*---------------------------------
    CLASSI INTERFACE PER HAL
----------------------------------*/
class HAL_IMU {
public:
    virtual bool init() = 0;
    virtual void calib() = 0;
    virtual Vector3f getRawGyro() = 0;
    virtual Vector3f getRawAccel() = 0;
    virtual ImuData getRawImu() = 0;
};

class HAL_GPS {
public:
#ifdef HAL_TEENSY
    virtual bool init(Stream *serialPtr) = 0;
#endif
    virtual bool init() = 0;
    virtual GpsData read() = 0;
};

class HAL_MAG {
public:
    virtual bool init() = 0;
    virtual void read() = 0;
};

class HAL_BARO {
public:
    virtual void read() = 0;
};

class HAL_LIDAR {
public:
#ifdef HAL_TEENSY
    virtual bool init(Stream *serialPtr) = 0;
#endif
    virtual bool init() = 0;
    virtual void calib() = 0;
    virtual LidarData read() = 0;
};

class HAL_MOTOR {
public:
    virtual void init() = 0;
    virtual void write(float m1, float m2, float m3, float m4) = 0;
    virtual void stop() = 0;
    virtual void test() = 0;
};

class HAL_Logging {
public:
    virtual unsigned long long writeLog() = 0;
};

class HAL_Telemetry {
public:
    virtual unsigned long long micros() = 0;
};

/**
 * HAL che gestisce il time e le interrupts
 */
class HAL_Time_Interrupts {
public:
    virtual unsigned long long micros() = 0;

    virtual void enterCritical() = 0;
    virtual void exitCritical() = 0;

    virtual void startHardLoop(uint32_t frequency, void (*callback)()) = 0;
};


class HAL{
public:
    HAL();
    ~HAL();

    HALState init();

    bool registerImu(HAL_IMU* imu_instance);        // Metodo per registare una nuova  IMU
    HAL_IMU* getImuInstance(uint8_t idx);           // Metodo per prendere l'instanza di una IMU

    bool registerGps(HAL_GPS* gps_instance);
    HAL_GPS* getGpsInstance(uint8_t idx);

    bool registerLidar(HAL_LIDAR* lidar_instance);
    HAL_LIDAR* getLidarInstance(uint8_t idx);

    bool registerMag(HAL_MAG* mag_instance);
    HAL_MAG* getMagInstance(uint8_t idx);

    bool registerBaro(HAL_BARO* baro_instance);
    HAL_BARO* getBaroInstance(uint8_t idx);

    HAL_MOTOR* motor;
    HAL_Telemetry* telemetry;
    HAL_Logging* logger;
    HAL_Time_Interrupts* time;

private:
    HAL_IMU* _imu_instances[IMU_INSTANCES];         // Buffer per le instanze delle IMU
    uint8_t _imu_count;                             // Contatore per le imu aggiunte

    HAL_GPS* _gps_instances[GPS_INSTANCES];         // Buffer per le instanze del GPS
    uint8_t _gps_count;                             // Contatore per il numero di gps aggiunti

    HAL_LIDAR* _lidar_instances[LIDAR_INSTANCES];   // Buffer per le instanze dei lidar
    uint8_t _lidar_count;                           // Contatore per il numero di lidar

    HAL_MAG* _mag_instances[MAG_INSTANCES];
    uint8_t _mag_count;

    HAL_BARO* _baro_instances[BARO_INSTANCES];
    uint8_t _baro_count;

    void _multi_instances_reset();      // Funzione che inserisce nullptr in tutte le celle degli array delle instanze dei sensori.

};


// Include delle implementazioni concrete DOPO le definizioni delle interfacce
#ifdef HAL_TEENSY
    #include <hal/teensy/HAL_Teensy.hpp>

    #include <hal/teensy/drivers/imu/Bmi088_driver/Bmi088_driver.hpp>
    #include <hal/teensy/drivers/lidar/Lidar.hpp>
    #include <hal/teensy/drivers/motor/Motor.hpp>
    #include <hal/teensy/drivers/gps/Gps.hpp>
    #include <hal/teensy/drivers/mag/Mag.hpp>
#elif defined(HAL_SITL)
    #include <hal/SITL/HAL_sitl.hpp>
#endif;