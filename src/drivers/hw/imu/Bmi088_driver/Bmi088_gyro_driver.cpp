#include "Bmi088_gyro_driver.hpp"
#include "log_instance_pool.hpp"

// Definisco il pool per questo modulo
LOG_INSTANCE_POOL_DEFINE(imu_bmi088_gyro, NUM_BMI088_INSTANCES)

Bmi088_gyro_driver::Bmi088_gyro_driver(const char* model,uint8_t instance_id, const struct device *gyro_dev):
    model_(model), 
    instance_id_(instance_id),
    id_(0xFF),
    gyro_dev_(gyro_dev)
{
    init_logging(imu_bmi088_gyro_log_instance_get(instance_id));

    trig_gyro_ = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_GYRO_XYZ,
    };
};

Bmi088_gyro_driver::~Bmi088_gyro_driver(){}

bool Bmi088_gyro_driver::init(uint8_t unique_id, srimb::RawGyroTopic& topic, WorkQueue& wq){
    unique_id_ = unique_id;
    raw_gyro_topic_ = &topic;
    fast_sensors_wq_ = &wq;

    if (!device_is_ready(gyro_dev_)) {
        log_err("Gyro device not ready!");
        return false;
    }

    log_inf("Gyro ready.");
    
    if(sensor_trigger_set(gyro_dev_, &trig_gyro_, gyro_isr_handler) < 0){
        log_err("Impossibile impostare il trigger gyro!");
        return false;
    }

    log_inf("Gyro's trigger configured."); // capire se metterli dbg
    return true; 
}

void Bmi088_gyro_driver::handler() {
    uint64_t timestamp_us = k_cyc_to_us_floor64(k_cycle_get_64());

    struct sensor_value gyro[3];
    struct sensor_value temp_value;

    if (sensor_sample_fetch(gyro_dev_) < 0) {
        // Errore, capire come gestirlo
        return;
    }

    sensor_channel_get(gyro_dev_, SENSOR_CHAN_GYRO_XYZ, gyro);
    sensor_channel_get(gyro_dev_, SENSOR_CHAN_DIE_TEMP, &temp_value);

    RawGyroData data = {
        .timestamp = timestamp_us,
        .id = unique_id_,
        // aggiunger anche il modello
        .x = sensor_value_to_float(&gyro[0]),
        .y = sensor_value_to_float(&gyro[1]),
        .z = sensor_value_to_float(&gyro[2]),

        .temperature = sensor_value_to_float(&temp_value)
    };
    raw_gyro_topic_->publish(data);
}

