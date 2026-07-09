#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "uav_types.hpp"

class Bmi088_gyro_driver : public WorkItemBase<Bmi088_gyro_driver>
{
public:
    Bmi088_gyro_driver(const char* model, const struct device *gyro_dev, const gpio_dt_spec* gyro_int)
                        : model_(model), id_(0xFF),
                        gyro_dev_(gyro_dev),
                        gyro_int_(gyro_int)
                        {};
    
    ~Bmi088_gyro_driver() {

    };

    bool init () {

    }:

private:
    WorkQueue* fast_sensors_wq_;
    srimb::SRIMBTopic<RawGyroData>* raw_gyro_topic_;

    uint8_t id_;
    const char* model_;

    const struct device *gyro_dev_;
    struct gpio_dt_spec gyro_int_;
    struct gpio_callback gyro_cb_;
};
