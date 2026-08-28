#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "HAL_configs.hpp"
#include "Loggable.hpp"

#include "WorkItem.hpp"
#include "WorkQueue.hpp"

#include "data_types/DataTypes.h"

#include "srimb_topics/TopicTypes.h"

class Bmi088_acc_driver : public WorkItemBase<Bmi088_acc_driver>, public Loggable
{
public:
    Bmi088_acc_driver(const char* model, uint8_t instance_id, const struct device *accel_dev);
                        
    ~Bmi088_acc_driver();

    bool init(uint8_t unique_id, srimb::RawAccTopic& topic, WorkQueue& wq);

    // Eseguito nella work queue
    void handler();

private:
    WorkQueue* fast_sensors_wq_ {};
    srimb::RawAccTopic* raw_acc_topic_ {};

    uint8_t unique_id_;
    uint8_t instance_id_;
    const char* model_;

    const struct device *accel_dev_;
    struct sensor_trigger trig_accel_ {};

    static void accel_isr_handler(const struct device *dev, const struct sensor_trigger *trig) {
        Bmi088_acc_driver *self = CONTAINER_OF(trig, Bmi088_acc_driver, trig_accel_);
        self->submitTo(*self->fast_sensors_wq_);
    }

};
