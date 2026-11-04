#pragma once

/*-------------------------------
	WRAPPER PER IL BMI088
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "HAL.hpp"
#include "SRIMB.hpp"

#include "Bmi088_acc_driver.hpp"
#include "Bmi088_gyro_driver.hpp"


class Bmi088_driver : public IHAL_IMU
{

public:
	Bmi088_driver(const char* model, uint8_t instance_id, const struct device *accel_dev, const struct device *gyro_dev):
		IHAL_IMU(model),
		acc_driver_(model, instance_id, accel_dev),
		gyro_driver_(model, instance_id, gyro_dev)
	{};

	~Bmi088_driver() = default;

	bool init(uint8_t unique_id, srimb::SRIMBTopicManager& topicManager, WorkQueue& wq) override {
		set_id(unique_id);

		bool gyro_status = gyro_driver_.init(unique_id, topicManager, wq);
		bool acc_status = acc_driver_.init(unique_id, topicManager, wq);

		return (gyro_status && acc_status);
	};


private:
	Bmi088_acc_driver acc_driver_ ;
	Bmi088_gyro_driver gyro_driver_ ;
};
