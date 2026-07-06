#pragma once

/*-------------------------------
    WRAPPER PER IL BMI088
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

#include <cstdint>
#include "HAL.hpp"
#include "Vector3f.h"
#include "Quaternion.h"
// #include <config/board_configs.h>
// #include <config/parameters.h>
#include "uav_types.hpp"


class Bmi088_driver : public IHAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    
    const struct device *_gyro_dev;
    const struct device *_accel_dev;

    double _get_imu_temp(); 
    
public:
    Bmi088_driver(const uint8_t id, const char* name, const struct device *accel_dev, const struct device *gyro_dev) : 
                        IHAL_IMU(id, name),
                        _accel_dev(accel_dev), 
                        _gyro_dev(gyro_dev)  {};

    ~Bmi088_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f get_raw_gyro() override;
    Vector3f get_raw_accel() override;
    ImuData get_imu() override;

    
};
