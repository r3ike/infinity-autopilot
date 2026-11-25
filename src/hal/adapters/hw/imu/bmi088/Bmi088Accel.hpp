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
#include "interfaces/IStreamingDevice.hpp"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>

class Bmi088Accel : public WorkItem<Bmi088Accel>,public IStreamingDevice ,public Loggable
{
public:
	Bmi088Accel(std::string_view device_name, const struct device *accel_dev);
	~Bmi088Accel() = default;

	bool init (uint8_t instance_id, HalContext& ctx) override;

	bool start() override;
	bool stop() override;

	// Executed in the workqueue
	void handler();
private:
	WorkQueue* fast_sensors_wq_ {};
	srimb::SRIMBPub<srimb::RawAccTopic> raw_acc_topic_pub_;

	std::string_view device_name_;
	uint32_t failed_fetch_count_{0};	// Number of failed fetch from the sensor

	const struct device *accel_dev_;
	struct sensor_trigger trig_accel_ {};

	static void accel_isr_handler(const struct device *dev, const struct sensor_trigger *trig) {
		Bmi088Accel *self = CONTAINER_OF(trig, Bmi088Accel, trig_accel_);
		self->submitTo(*self->fast_sensors_wq_);
	}
};



