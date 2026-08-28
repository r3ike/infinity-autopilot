#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "HAL_configs.hpp"
#include "Loggable.hpp"

#include "WorkQueue.hpp"
#include "WorkItem.hpp"
#include "data_types/DataTypes.h"

#include "srimb_topics/TopicTypes.h"

class Bmi088_gyro_driver : public WorkItemBase<Bmi088_gyro_driver>, public Loggable
{
public:
    Bmi088_gyro_driver(const char* model,uint8_t instance_id, const struct device *gyro_dev);
    
    ~Bmi088_gyro_driver();

    bool init (uint8_t unique_id, srimb::RawGyroTopic& topic, WorkQueue& wq);

    // Eseguito nella work queue
    void handler();

private:
    WorkQueue* fast_sensors_wq_;
    srimb::RawGyroTopic* raw_gyro_topic_;

    uint8_t unique_id_;
    uint8_t instance_id_;
    const char* model_;

    const struct device *gyro_dev_;
    struct sensor_trigger trig_gyro_ {};

    static void gyro_isr_handler(const struct device *dev, const struct sensor_trigger *trig) {
        Bmi088_gyro_driver *self = CONTAINER_OF(trig, Bmi088_gyro_driver, trig_gyro_);
        self->submitTo(*self->fast_sensors_wq_);
    }

    
};
