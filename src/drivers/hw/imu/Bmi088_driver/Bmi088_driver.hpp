#pragma once

/*-------------------------------
    WRAPPER PER LA IMU
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <HAL.hpp>
#include <Vector3f.h>
#include <Quaternion.h>
// #include <config/board_configs.h>
// #include <config/parameters.h>
#include <srimb_topics/imu_topic/imu_topic.hpp>


class Bmi088_driver : public IHAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    
    const struct device *_gyro_dev;
    const struct device *_accel_dev;

    double _getImuTemp(); 
    
public:
    Bmi088_driver(const struct device *accel_dev, const struct device *gyro_dev);
    ~Bmi088_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f getRawGyro() override;
    Vector3f getRawAccel() override;
    ImuData getRawImu() override;

    
};
