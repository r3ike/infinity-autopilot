#include "Bmi088Accel.hpp"
#include "log_instance_pool.hpp"
#include "Bmi088_config.hpp"

LOG_INSTANCE_POOL_DEFINE(imu_bmi088_acc, NUM_BMI088_INSTANCES)

Bmi088Accel::Bmi088Accel(std::string_view device_name, const device *accel_dev) :
	device_name_(device_name),
	accel_dev_(accel_dev)
{
	mark_device_state(DeviceState::Uninitialized);
	trig_accel_ = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ACCEL_XYZ,
	};
}

bool Bmi088Accel::init(uint8_t instance_id, HalContext &ctx)
{
	mark_device_state(DeviceState::Initializing);
    instance_id_ = instance_id;
	assign_identity(instance_id, device_name_);
	init_logging(imu_bmi088_acc_log_instance_get(instance_id));

	raw_acc_topic_pub_.setTopic(ctx.topicManager.getOrCreateTopic<srimb::RawAccTopic>(instance_id));
	fast_sensors_wq_ = &ctx.fast_sensors_wq;

	if (!device_is_ready(accel_dev_)) {
		log_err("Accel device not ready!");
		mark_device_state(DeviceState::Failed);
		return false;
	}

	log_inf("Accel ready.");    // capire se metterli dbg

	if(sensor_trigger_set(accel_dev_, &trig_accel_, accel_isr_handler) < 0){
		log_err("Impossibile impostare il trigger acc!");
		mark_device_state(DeviceState::Failed);
		return false;
	}

	log_inf("Accel's trigger configured."); // capire se metterli dbg
	mark_device_state(DeviceState::Ready);
	return true; 
}

void Bmi088Accel::handler()
{
	uint64_t timestamp_us = k_cyc_to_us_floor64(k_cycle_get_64());

	struct sensor_value accel[3];

	if (sensor_sample_fetch(accel_dev_) < 0) {
		failed_fetch_count_++;
		log_err("Error in the fetch.");
		return;
	}

	sensor_channel_get(accel_dev_, SENSOR_CHAN_ACCEL_XYZ, accel);

	RawAccData data = {
		.timestamp = timestamp_us,
		.id = instance_id_,
		// aggiunger anche il modello
		.x = sensor_value_to_float(&accel[0]),
		.y = sensor_value_to_float(&accel[1]),
		.z = sensor_value_to_float(&accel[2])
	};

	raw_acc_topic_pub_.publish(data);
}
