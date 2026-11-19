#include "Bmi088Gyro.hpp"
#include "Bmi088_config.hpp"
#include "log_instance_pool.hpp"

// Definisco il pool per questo modulo

LOG_INSTANCE_POOL_DEFINE(imu_bmi088_gyro, NUM_BMI088_INSTANCES);

Bmi088Gyro::Bmi088Gyro(std::string_view device_name, const struct device *gyro_dev) :
	device_name_(device_name),
	gyro_dev_(gyro_dev)
{
	mark_device_state(DeviceState::Uninitialized);
	trig_gyro_ = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_GYRO_XYZ,
	};
}

bool Bmi088Gyro::init(uint8_t instance_id, HalContext& ctx)
{
	mark_device_state(DeviceState::Initializing);
    instance_id_ = instance_id;
	assign_identity(instance_id, device_name_);
	init_logging(imu_bmi088_gyro_log_instance_get(instance_id));

	raw_gyro_topic_pub_.setTopic(ctx.topicManager.getOrCreateTopic<srimb::RawGyroTopic>(instance_id));
	fast_sensors_wq_ = &ctx.fast_sensors_wq;


	if (!device_is_ready(gyro_dev_)) {
		log_err("Gyro device not ready!");
		mark_device_state(DeviceState::Failed);
		return false;
	}

	log_inf("Gyro ready.");
	
	if(sensor_trigger_set(gyro_dev_, &trig_gyro_, gyro_isr_handler) < 0){
		log_err("Impossibile impostare il trigger gyro!");
		mark_device_state(DeviceState::Failed);
		return false;
	}

	log_inf("Gyro's trigger configured."); // capire se metterli dbg
	mark_device_state(DeviceState::Ready);
	return true; 
}

void Bmi088Gyro::handler() {
	uint64_t timestamp_us = k_cyc_to_us_floor64(k_cycle_get_64());

	struct sensor_value gyro[3];
	struct sensor_value temp_value;

	if (sensor_sample_fetch(gyro_dev_) < 0) {
		failed_fetch_count_++;
		log_err("Error in the fetch.");
		return;
	}

	sensor_channel_get(gyro_dev_, SENSOR_CHAN_GYRO_XYZ, gyro);
	sensor_channel_get(gyro_dev_, SENSOR_CHAN_DIE_TEMP, &temp_value);

	RawGyroData data = {
		.timestamp = timestamp_us,
		.id = instance_id_,
		// aggiunger anche il modello
		.x = sensor_value_to_float(&gyro[0]),
		.y = sensor_value_to_float(&gyro[1]),
		.z = sensor_value_to_float(&gyro[2]),

		.temperature = sensor_value_to_float(&temp_value)
	};
	raw_gyro_topic_pub_.publish(data);
}
