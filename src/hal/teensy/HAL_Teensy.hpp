#pragma once
#include <hal/HAL.hpp>
#include <Arduino.h>

//#include <hal/teensy/drivers/imu/Imu.h>
#include <hal/teensy/drivers/lidar/Lidar.hpp>
#include <hal/teensy/drivers/motor/Motor.hpp>
#include <hal/teensy/drivers/mag/Mag.hpp>


/*--------------------------
        HAL IMU
---------------------------*/
/*
class HAL_IMU_Teensy : public HAL_IMU
{
private:
    Imu _imu;
public:
    HAL_IMU_Teensy();
    ~HAL_IMU_Teensy() = default;

    bool init() override;

    void calib() override;

    Vector3f readGyro() override;
    Vector3f readAccel() override;
    ImuData read() override;
};
*/

/*--------------------------
        HAL PWM
---------------------------*/
class HAL_MOTOR_Teensy : public HAL_MOTOR
{
private:
    Motor _motors;
public:
    HAL_MOTOR_Teensy();
    ~HAL_MOTOR_Teensy();

    void init() override;
    void test() override;
    void stop() override;
    void write(float m1, float m2, float m3, float m4) override;
};

/*--------------------------
        HAL MAG
---------------------------*/
class HAL_MAG_Teensy : public HAL_MAG
{
private:
    Mag _mag;
public:
    HAL_MAG_Teensy();
    ~HAL_MAG_Teensy();

    bool init() override;
    void read() override;
};

/*--------------------------
        HAL GPS
---------------------------*/
/*
class HAL_GPS_Teensy : public HAL_GPS
{
private:
    Gps _gps;
public:
    HAL_GPS_Teensy();
    ~HAL_GPS_Teensy();

    bool init(Stream *serialPtr) override;
    bool init() override;
    GpsData read() override;
};
*/

/*--------------------------
        HAL LIDAR
---------------------------*/
class HAL_LIDAR_Teensy : public HAL_LIDAR
{
private:
    Lidar _lidar;
public:
    HAL_LIDAR_Teensy();
    ~HAL_LIDAR_Teensy();

    bool init(Stream *serialPtr) override;
    bool init() override;
    void calib() override;
    LidarData read() override;
};

/*--------------------------
        HAL TIME
---------------------------*/
class HAL_TIME_INTERRUPTS_Teensy : public HAL_Time_Interrupts{
    private:
        IntervalTimer _timer;
    public:
        HAL_TIME_INTERRUPTS_Teensy();
        ~HAL_TIME_INTERRUPTS_Teensy() = default;

        unsigned long long micros() override;

        void enterCritical() override { noInterrupts(); }
        void exitCritical() override { interrupts(); }

        void startHardLoop(uint32_t frequency, void (*callback)()) override;


};