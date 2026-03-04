#pragma once

/*-------------------------------
    WRAPPER PER LA IMU
-------------------------------*/

#include <Arduino.h>
#include <Wire.h>
#include <hal/teensy/lib/BMI088/BMI088.h>
#include <hal/HAL.hpp>

#include <utils/Vector3f.h>
#include <utils/Quaternion.h>
#include <config/board_configs.h>
#include <config/parameters.h>
#include <utils/srimb_topics/imu_topic/imu_topic.hpp>


class Bmi088_driver : public HAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    BMI088 _bmi088 = BMI088(BMI088_ACC_ADDRESS, BMI088_GYRO_ADDRESS);

    bool _connect();

    int16_t _getImuTemp(); 
    
public:
    Bmi088_driver();
    ~Bmi088_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f getRawGyro() override;
    Vector3f getRawAccel() override;
    ImuData getRawImu() override;

    
};
