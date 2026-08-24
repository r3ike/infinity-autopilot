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
#include "Loggable.hpp"

class Bmi088_acc_driver : public WorkItemBase<Bmi088_acc_driver>, public Loggable
{
public:
    Bmi088_acc_driver(const char* model, const struct device *accel_dev)
                        :model_(model), id_(0xFF),
                        accel_dev_(accel_dev)
                        {
                            trig_accel_ = {
                                .type = SENSOR_TRIG_DATA_READY,
                                .chan = SENSOR_CHAN_ACCEL_XYZ,
                            };
                        };
    
    
    ~Bmi088_acc_driver() {
        
    };

    bool init(uint8_t unique_id, srimb::SRIMBTopic<RawAccData>& topic, WorkQueue& wq) {
        id_ = unique_id;

        raw_acc_topic_ = &topic;
        fast_sensors_wq_ = &wq;

        init_logging(model_, id_);

        if (!device_is_ready(accel)) {
            log_err("Accel device not ready!");
            return false;
        }

        log_inf("Accel ready.");    // capire se metterli dbg

        if(sensor_trigger_set(accel_, &trig_accel_, accel_isr_handler) < 0){
            log_err("Impossibile impostare il trigger acc!");
            return false;
        }

        log_inf("Accel's trigger configured."); // capire se metterli dbg

        return true;
    };

    // Eseguito nella work queue
    void handler() {
        uint64_t timestamp_us = k_cyc_to_us_floor64(k_cycle_get_64());

        struct sensor_value accel[3];

        if (sensor_sample_fetch(accel_dev_) < 0) {
            // Errore, capire come gestirlo
            log_err("Errore nella lettura del sample.");
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

        raw_acc_topic_->publish(data);
    }

private:
    WorkQueue* fast_sensors_wq_ = nullptr;
    srimb::SRIMBTopic<RawAccData>* raw_acc_topic_ = nullptr;

    uint8_t id_;
    const char* model_;

    const struct device *accel_dev_;
    
    struct sensor_trigger trig_accel_ = nullptr;

    static void accel_isr_handler(const struct device *dev, const struct sensor_trigger *trig) {
        Bmi088_acc_driver *self = CONTAINER_OF(trig, Bmi088_acc_driver, trig_accel_);
        self->submitTo(*self->fast_sensors_wq_);
    }

};
