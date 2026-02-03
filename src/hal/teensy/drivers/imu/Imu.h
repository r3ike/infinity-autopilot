#ifndef IMU_H
#define IMU_H

/*-------------------------------
    WRAPPER PER LA IMU
-------------------------------*/

#include <Wire.h>
#include <./hal/teensy/lib/BMI088/BMI088.h>

#include <utils/Vector3f.h>
#include <utils/Quaternion.h>
#include <config/board_configs.h>
#include <config/parameters.h>
#include <utils/uav_data.hpp>


class Imu
{
private:
    Vector3f _gyro_rate_calib;
    BMI088 _bmi088 = BMI088(BMI088_ACC_ADDRESS, BMI088_GYRO_ADDRESS);

    bool connect();
    
public:
    Imu();
    ~Imu();

    bool init();
    void calib();

    Vector3f getRawGyro();
    Vector3f getRawAccel();
    ImuData getRawImu();

    int16_t getImuTemp();
};

#endif