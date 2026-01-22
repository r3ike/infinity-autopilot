#include "hal/HAL.hpp"
#include "HAL.hpp"

HAL createHAL(){
    HAL hal;

    #ifdef HAL_TEENSY
        hal.imu = new HAL_IMU_Teensy();
        hal.motor = new HAL_MOTOR_Teensy();
        //hal.telemetry = new HAL_Telemetry_Teensy();
        //hal.logging = new HAL_Logging_Teensy();
        //hal.time = new HAL_Time_Teensy();
    #elif defined(HAL_SITL)
        hal.imu = new HAL_IMU_SITL();
        //hal.pwm = new HAL_MOTOR_SITL();
        //hal.telemetry = new HAL_Telemetry_SITL();
        //hal.logging = new HAL_Logging_SITL();
        //hal.time = new HAL_Time_SITL();
    #endif

    return hal;
}

HAL::HAL()
{
    #ifdef HAL_TEENSY
        imu = new HAL_IMU_Teensy();
        motor = new HAL_MOTOR_Teensy();
        gps = new HAL_GPS_Teensy();
        lidar = new HAL_LIDAR_Teensy();
        mag = new HAL_MAG_Teensy();
        //telemetry = new HAL_Telemetry_Teensy();
        //logging = new HAL_Logging_Teensy();
        //time = new HAL_Time_Teensy();
    #elif defined(HAL_SITL)
        imu = new HAL_IMU_SITL();
        //pwm = new HAL_MOTOR_SITL();
        //telemetry = new HAL_Telemetry_SITL();
        //logging = new HAL_Logging_SITL();
        //time = new HAL_Time_SITL();
    #endif
}
HAL::~HAL(){}

HALState HAL::init()
{
    HALState states;
    #ifdef HAL_TEENSY
        Serial4.begin(9600);
        Serial2.begin(9600);
        states.imu_state = imu->init();
        states.gps_state = gps->init(&Serial4);
        states.lidar_state = lidar->init(&Serial2);

    #elif defined(HAL_SITL)

    #endif

    return states;
}
