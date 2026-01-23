#pragma once


#ifdef HAL_TEENSY
    #include <Arduino.h>
#endif

#include <utils/uav_data.hpp>
#include <utils/Vector3f.h>
#include <config/board_configs.h>

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

class HAL_Time {
public:
    virtual unsigned long long micros() = 0;
};


class HAL{
public:
    HAL();
    ~HAL();

    HALState init();

    HAL_IMU* imu;
    HAL_GPS* gps;
    HAL_LIDAR* lidar;
    HAL_MAG* mag;
    HAL_MOTOR* motor;
    HAL_Telemetry* telemetry;
    HAL_Logging* logger;
    HAL_Time* time;
};

HAL createHAL();

// Include delle implementazioni concrete DOPO le definizioni delle interfacce
#ifdef HAL_TEENSY
    #include <hal/teensy/HAL_Teensy.hpp>
#elif defined(HAL_SITL)
    #include <hal/SITL/HAL_sitl.hpp>
#endif