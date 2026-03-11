#pragma once

/*-------------------------------
    WRAPPER PER IL ICM42688P
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
#include "imu_topic/imu_topic.hpp"

class ICM42688P_driver : public IHAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    
    const struct device *_imu_dev;

    std::unique_ptr<ImuLpfFilter> _lpf_filter;

    double _getImuTemp(); 
public:
    ICM42688P_driver();
    ~ICM42688P_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f getRawGyro() override;
    Vector3f getRawAccel() override;
    ImuData getRawImu() override;
};



