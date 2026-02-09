#pragma once


#ifdef HAL_TEENSY
    #include <Arduino.h>
#endif

#include <utils/uav_data.hpp>
#include <utils/Vector3f.h>
#include <config/board_configs.h>
#include <config/parameters.h>

struct HALState{
    bool imu_state;
    bool motor_state;
    bool mag_state;
    bool lidar_state;
    bool gps_state;
    bool tlm_state;
    bool loger_state;
};



/*---------------------------------
    CLASSI INTERFACE PER HAL
----------------------------------*/
class HAL_IMU {
public:
    virtual bool init() = 0;
    virtual void calib() = 0;
    virtual Vector3f readGyro() = 0;
    virtual Vector3f readAccel() = 0;
    virtual ImuData read() = 0;
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

    bool registerImu(HAL_IMU* imu_instance);
    HAL_IMU* getImuInstance(uint8_t idx);

    HAL_MOTOR* motor;
    HAL_Telemetry* telemetry;
    HAL_Logging* logger;
    HAL_Time_Interrupts* time;

private:
    HAL_IMU* _imus[IMU_INSTANCES];
    HAL_GPS* _gps[GPS_INSTANCES];
    HAL_LIDAR* _lidars[LIDAR_INSTANCES];
    HAL_MAG* _mags[MAG_INSTANCES];
    HAL_BARO* _baros[BARO_INSTANCES];

};


// Include delle implementazioni concrete DOPO le definizioni delle interfacce
#ifdef HAL_TEENSY
    #include <hal/teensy/HAL_Teensy.hpp>
#elif defined(HAL_SITL)
    #include <hal/SITL/HAL_sitl.hpp>
#endif