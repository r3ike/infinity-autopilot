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

#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "HAL.hpp"
#include "Vector3f.h"
#include "Quaternion.h"
#include "uav_types.hpp"


class Bmi088_driver : public IHAL_IMU, public WorkItemBase<Bmi088_driver>
{

    
public:
    Bmi088_driver(const uint8_t id, const char* model, const struct device *accel_dev, 
                  const struct device *gyro_dev, const gpio_dt_spec* accel_int, 
                  const gpio_dt_spec* gyro_int,
                  srimb::SRIMBTopic<ImuData>& topic, WorkQueue &wq) 
          : IHAL_IMU(id, model),
            _accel_dev(accel_dev), 
            _gyro_dev(gyro_dev),
            accel_int_(accel_int),
            gyro_int_(gyro_int)
        {};

    ~Bmi088_driver() = default;

    bool init() override {

    };


private:
    WorkQueue fast_sensors_wq_;
    srimb::SRIMBTopic<ImuData>& raw_topic_;
    
    const struct device *_gyro_dev;
    const struct device *_accel_dev;
    struct gpio_dt_spec accel_int_;
    struct gpio_dt_spec gyro_int_;
    struct gpio_callback accel_cb_;
    struct gpio_callback gyro_cb_;

    double _get_imu_temp(); 

    
};
