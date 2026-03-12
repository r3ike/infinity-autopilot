#pragma once

/*-------------------------------
    WRAPPER PER IL BMI088
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

#include "ImuLpfFilter.hpp"
#include "HAL.hpp"
#include "Vector3f.h"
#include "Quaternion.h"
// #include <config/board_configs.h>
// #include <config/parameters.h>
#include "imu_topic/imu_topic.hpp"


class Bmi088_driver : public IHAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    
    const struct device *_gyro_dev;
    const struct device *_accel_dev;

    std::unique_ptr<ImuLpfFilter> _lpf_filter;

    double _get_imu_temp(); 
    
public:
    Bmi088_driver(const struct device *accel_dev, const struct device *gyro_dev);
    ~Bmi088_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f get_raw_gyro() override;
    Vector3f get_raw_accel() override;
    ImuData get_imu() override;

    
};
