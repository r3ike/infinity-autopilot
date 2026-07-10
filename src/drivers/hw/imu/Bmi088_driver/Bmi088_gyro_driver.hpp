#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "WorkQueue.hpp"
#include "WorkItem.hpp"
#include "SRIMB.hpp"
#include "uav_types.hpp"

class Bmi088_gyro_driver : public WorkItemBase<Bmi088_gyro_driver>
{
public:
    Bmi088_gyro_driver(const char* model, const struct device *gyro_dev, const gpio_dt_spec* gyro_int)
                        : model_(model), id_(0xFF),
                        gyro_dev_(gyro_dev),
                        gyro_int_(*gyro_int)
                        {};
    
    ~Bmi088_gyro_driver() {
        if (gyro_int_.port) {
            gpio_remove_callback(gyro_int_.port, &gyro_cb_);
        }
    };

    bool init (uint8_t unique_id, srimb::SRIMBTopic<RawGyroData>& topic, WorkQueue& wq) {
        id_ = unique_id;

        raw_gyro_topic_ = &topic;
        fast_sensors_wq_ = &wq;

        // Configura interrupt giroscopio
        if (gyro_int_.port != NULL) {
            int ret = gpio_pin_configure_dt(&gyro_int_, GPIO_INPUT);
            if (ret < 0) return false;

            ret = gpio_pin_interrupt_configure_dt(&gyro_int_, GPIO_INT_EDGE_TO_ACTIVE);
            if (ret < 0) return false;

            gpio_init_callback(&gyro_cb_, gyro_isr_handler, BIT(gyro_int_.pin));
            gpio_add_callback(gyro_int_.port, &gyro_cb_);

            return true;
        }
        
        return false;
    };

    // Eseguito nella work queue
    void handler() {
        uint64_t timestamp_us = k_cyc_to_us_floor32(k_cycle_get_32());

        struct sensor_value gyro[3];
        struct sensor_value temp_value;

        if (sensor_sample_fetch(gyro_dev_) < 0) {
            // Errore, capire come gestirlo
            return;
        }
        sensor_channel_get(gyro_dev_, SENSOR_CHAN_ACCEL_XYZ, gyro);
        sensor_channel_get(gyro_dev_, SENSOR_CHAN_DIE_TEMP, &temp_value);

        RawGyroData data = {
            .timestamp = timestamp_us,
            .id = id_,
            // aggiunger anche il modello

            .x = sensor_value_to_float(&gyro[0]),
            .y = sensor_value_to_float(&gyro[1]),
            .z = sensor_value_to_float(&gyro[2]),

            .temperature = sensor_value_to_float(&temp_value)
        };

        raw_gyro_topic_->publish(data, timestamp_us);
    }

private:
    WorkQueue* fast_sensors_wq_;
    srimb::SRIMBTopic<RawGyroData>* raw_gyro_topic_;

    uint8_t id_;
    const char* model_;

    const struct device *gyro_dev_;
    struct gpio_dt_spec gyro_int_;
    struct gpio_callback gyro_cb_;

    static void gyro_isr_handler(const struct device *port, struct gpio_callback *cb, uint32_t pins) {
        Bmi088_gyro_driver *self = CONTAINER_OF(cb, Bmi088_gyro_driver, gyro_cb_);
        self->submitTo(*self->fast_sensors_wq_);
    }

    
};
