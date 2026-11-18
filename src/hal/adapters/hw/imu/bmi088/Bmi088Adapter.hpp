#pragma once
/*-------------------------------
	ADAPTER PER IL BMI088
-------------------------------*/
#include <cstdint>
#include <string_view>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/autoconf.h>


#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"

#include "factory/HalContext.hpp"
#include "interfaces/IImu.hpp"

#include "Bmi088Accel.hpp"
#include "Bmi088Gyro.hpp"


class Bmi088Adapter : public IImu
{

public:
	Bmi088Adapter(std::string_view model, const struct device *accel_dev, const struct device *gyro_dev);
	~Bmi088Adapter() = default;

	bool init(uint8_t instance_id, HalContext& ctx) override;
private:
	Bmi088Gyro bmi088_gyro_;

	std::string_view model_name_;
};



class Bmi088_driver : public IHAL_IMU
{

public:
	Bmi088_driver(const char* model, uint8_t id, const struct device *accel_dev, const struct device *gyro_dev):
		IHAL_IMU(model),
		acc_driver_(model, id, accel_dev),
		gyro_driver_(model, id, gyro_dev)
	{};

	~Bmi088_driver() = default;

	bool init(uint8_t instance_id, srimb::SRIMBTopicManager& topicManager, WorkQueue& wq) override {
		set_id(instance_id);

		bool gyro_status = gyro_driver_.init(instance_id, topicManager, wq);
		bool acc_status = acc_driver_.init(instance_id, topicManager, wq);

		return (gyro_status && acc_status);
	};


private:
	Bmi088_acc_driver acc_driver_ ;
	Bmi088_gyro_driver gyro_driver_ ;
};
