#include "Bmi088_acc_driver.hpp"
#include "log_instance_pool.hpp"

// Definisco il pool per questo modulo
LOG_INSTANCE_POOL_DEFINE(imu_bmi088_acc, NUM_BMI088_INSTANCES)

Bmi088_acc_driver::Bmi088_acc_driver(const char* model, uint8_t instance_id, const struct device *accel_dev):
    model_(model), 
    instance_id_(instance_id),
    unique_id_(0xFF),
    accel_dev_(accel_dev)
{
    init_logging(imu_bmi088_log_instance_get(instance_id));

    trig_accel_ = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ACCEL_XYZ,
    };
};

Bmi088_acc_driver::~Bmi088_acc_driver(){}

bool Bmi088_acc_driver::init(uint8_t unique_id, srimb::RawAccTopic& topic, WorkQueue& wq) {
    id_ = unique_id;
    raw_acc_topic_ = &topic;
    fast_sensors_wq_ = &wq;

    //init_logging(model_, id_);

    if (!device_is_ready(accel_dev_)) {
        //log_err("Accel device not ready!");
        return false;
    }

    //log_inf("Accel ready.");    // capire se metterli dbg

    if(sensor_trigger_set(accel_dev_, &trig_accel_, accel_isr_handler) < 0){
        //log_err("Impossibile impostare il trigger acc!");
        return false;
    }

    //log_inf("Accel's trigger configured."); // capire se metterli dbg
    return true; 
}

void Bmi088_acc_driver::handler() {
    uint64_t timestamp_us = k_cyc_to_us_floor64(k_cycle_get_64());

    struct sensor_value accel[3];

    if (sensor_sample_fetch(accel_dev_) < 0) {
        // Errore, capire come gestirlo
        //log_err("Errore nella lettura del sample.");
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