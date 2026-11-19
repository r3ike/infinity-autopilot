#pragma once
#include <cstdint>
#include <string_view>
#include "Loggable.hpp"

#include "WorkQueue.hpp"
#include "WorkItem.hpp"
#include "data_types/DataTypes.h"
#include "srimb_topics/TopicTypes.h"

#include "SRIMB.hpp"
#include "SRIMBPub.hpp"

#include "factory/HalContext.hpp"
#include "interfaces/IDevice.hpp"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>

class Bmi088Gyro : public WorkItem<Bmi088Gyro>,public IDevice ,public Loggable
{
public:
	Bmi088Gyro(std::string_view device_name, const struct device *gyro_dev);
	~Bmi088Gyro() = default;

	bool init (uint8_t instance_id, HalContext& ctx) override;

	// Executed in the workqueue
	void handler();

private:
	WorkQueue* fast_sensors_wq_;
	srimb::SRIMBPub<srimb::RawGyroTopic> raw_gyro_topic_pub_;

	std::string_view device_name_;
	uint8_t instance_id_;
	uint32_t failed_fetch_count_{0};	// Number of failed fetch from the sensor

	const struct device *gyro_dev_;
	struct sensor_trigger trig_gyro_ {};

	static void gyro_isr_handler(const struct device *dev, const struct sensor_trigger *trig) {
		Bmi088Gyro *self = CONTAINER_OF(trig, Bmi088Gyro, trig_gyro_);
		self->submitTo(*self->fast_sensors_wq_);
	}
};

