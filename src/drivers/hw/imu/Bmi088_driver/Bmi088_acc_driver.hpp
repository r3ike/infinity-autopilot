#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "SRIMB.hpp"
#include "WorkItem.hpp"
#include "WorkQueue.hpp"

class Bmi088_acc_driver : public WorkItemBase<Bmi088_acc_driver>
{
public:
    Bmi088_acc_driver(const char* model, const struct device *accel_dev, const gpio_dt_spec* accel_int)
                        :model_(model), id_(0xFF),
                        accel_dev_(accel_dev),
                        accel_int_(*accel_int)
                        {};
    
    
    ~Bmi088_acc_driver() {
        if (accel_int_.port) {
            gpio_remove_callback(accel_int_.port, &accel_cb_);
        }
    };

    bool init(uint8_t unique_id, srimb::SRIMBTopic<RawAccData>& topic, WorkQueue& wq) {
        id_ = unique_id;

        raw_acc_topic_ = &topic;
        fast_sensors_wq_ = &wq;

        if (accel_dev_ == nullptr) {
            printk("gyro_dev null");
            return false;
        }

        if (!device_is_ready(accel_dev_)) {
            printk("Acc non pronto!!! \n");
            return false;
        }else{
            printk("Acc pronto!!! \n");
        }

        /*
        // Interrupt configuration
        if (accel_int_.port != NULL) {
            int ret = gpio_pin_configure_dt(&accel_int_, GPIO_INPUT);
            if (ret < 0) return false;

            ret = gpio_pin_interrupt_configure_dt(&accel_int_, GPIO_INT_EDGE_TO_ACTIVE);
            if (ret < 0) return false;

            gpio_init_callback(&accel_cb_, accel_isr_handler, BIT(accel_int_.pin));
            gpio_add_callback(accel_int_.port, &accel_cb_);

            return true;
        }*/

        return false;
    };

    // Eseguito nella work queue
    void handler() {
        uint64_t timestamp_us = k_cyc_to_us_floor32(k_cycle_get_32());

        struct sensor_value accel[3];

        if (sensor_sample_fetch(accel_dev_) < 0) {
            // Errore, capire come gestirlo
            return;
        }
        sensor_channel_get(accel_dev_, SENSOR_CHAN_ACCEL_XYZ, accel);

        RawAccData data = {
            .timestamp = timestamp_us,
            .id = id_,
            // aggiunger anche il modello

            .x = sensor_value_to_float(&accel[0]),
            .y = sensor_value_to_float(&accel[1]),
            .z = sensor_value_to_float(&accel[2])
        };

        raw_acc_topic_->publish(data, timestamp_us);
    }

private:
    WorkQueue* fast_sensors_wq_ = nullptr;
    srimb::SRIMBTopic<RawAccData>* raw_acc_topic_ = nullptr;

    uint8_t id_;
    const char* model_;

    const struct device *accel_dev_;
    struct gpio_dt_spec accel_int_;
    struct gpio_callback accel_cb_;

    static void accel_isr_handler(const struct device *port, struct gpio_callback *cb, uint32_t pins) {
        Bmi088_acc_driver *self = CONTAINER_OF(cb, Bmi088_acc_driver, accel_cb_);
        self->submitTo(*self->fast_sensors_wq_);
    }

    

};
