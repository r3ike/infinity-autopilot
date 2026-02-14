#include <hal/teensy/HAL_Teensy.hpp>
#include "HAL_Teensy.hpp"

/*--------------------------
        HAL IMU
---------------------------*/
/*
HAL_IMU_Teensy::HAL_IMU_Teensy(){}
HAL_IMU_Teensy::~HAL_IMU_Teensy(){}

bool HAL_IMU_Teensy::init()
{
    return _imu.init();
}

void HAL_IMU_Teensy::calib()
{
    _imu.calib();
}

Vector3f HAL_IMU_Teensy::readGyro() 
{
    return _imu.getRawGyro();
}

Vector3f HAL_IMU_Teensy::readAccel()
{
    return _imu.getRawAccel();
}

ImuData HAL_IMU_Teensy::read()
{
    return _imu.getRawImu();
}
*/

/*--------------------------
        HAL MOTOR
---------------------------*/

HAL_MOTOR_Teensy::HAL_MOTOR_Teensy(){}
HAL_MOTOR_Teensy::~HAL_MOTOR_Teensy(){}

void HAL_MOTOR_Teensy::init()
{
    _motors.init();
}

void HAL_MOTOR_Teensy::test()
{
    // Da implementare
}

void HAL_MOTOR_Teensy::stop()
{
    _motors.stop();
}

void HAL_MOTOR_Teensy::write(float m1, float m2, float m3, float m4)
{
    _motors.write((int)m1, (int)m2, (int)m3, (int)m4);
}
/*--------------------------
        HAL LIDAR
---------------------------*/
/*
HAL_LIDAR_Teensy::HAL_LIDAR_Teensy(){}
HAL_LIDAR_Teensy::~HAL_LIDAR_Teensy(){}

bool HAL_LIDAR_Teensy::init(Stream *serialPtr)
{
    return _lidar.init(serialPtr);
}

bool HAL_LIDAR_Teensy::init()
{
    return false;  // Fallback per init() senza parametri
}

void HAL_LIDAR_Teensy::calib()
{
    _lidar.calib();
}

LidarData HAL_LIDAR_Teensy::read()
{
    return _lidar.read();
}*/

/*--------------------------
        HAL GPS
---------------------------*/
/*
HAL_GPS_Teensy::HAL_GPS_Teensy(){}
HAL_GPS_Teensy::~HAL_GPS_Teensy(){}

bool HAL_GPS_Teensy::init(Stream *serialPtr)
{
    return _gps.init(serialPtr);
}

bool HAL_GPS_Teensy::init()
{
    return false;  // Fallback per init() senza parametri
}

GpsData HAL_GPS_Teensy::read()
{
    return _gps.read();
}*/

/*--------------------------
        HAL MAG
---------------------------*/
HAL_MAG_Teensy::HAL_MAG_Teensy(){}
HAL_MAG_Teensy::~HAL_MAG_Teensy(){}

bool HAL_MAG_Teensy::init()
{
    return _mag.init();
}

void HAL_MAG_Teensy::read()
{
    // TODO: Implementare lettura magnetometro
}

/*------------------------------------
        HAL TIME AND INTERRUPTS
------------------------------------*/

HAL_TIME_INTERRUPTS_Teensy::HAL_TIME_INTERRUPTS_Teensy(){}

unsigned long long HAL_TIME_INTERRUPTS_Teensy::micros()
{
    return micros();
}

void HAL_TIME_INTERRUPTS_Teensy::startHardLoop(uint32_t frequency, void (*callback)())
{
    unsigned long period_us = (unsigned long)(1000000.0f / frequency);
    _timer.begin(callback, period_us);
}
