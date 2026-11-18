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
#include "Loggable.hpp"

#include "factory/HalContext.hpp"
#include "interfaces/IImu.hpp"

#include "Bmi088Accel.hpp"
#include "Bmi088Gyro.hpp"



class Bmi088Adapter : public IImu, public Loggable
{

public:
	Bmi088Adapter(std::string_view device_name, const struct device *accel_dev, const struct device *gyro_dev);
	~Bmi088Adapter() = default;

	bool init(uint8_t instance_id, HalContext& ctx) override;
private:
	std::string_view device_name_;
	Bmi088Gyro bmi088_gyro_;
	Bmi088Accel bmi088_acc_;
};


