#pragma once

/*-------------------------------
    WRAPPER PER IL BMI088
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/autoconf.h>
#include <cstdint>

#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "HAL.hpp"
#include "uav_types.hpp"

#include "Bmi088_acc_driver.hpp"
#include "Bmi088_gyro_driver.hpp"


class Bmi088_driver : public IHAL_IMU
{

public:
    Bmi088_driver(const char* model, const struct device *accel_dev, 
                  const struct device *gyro_dev, const gpio_dt_spec* accel_int, 
                  const gpio_dt_spec* gyro_int) 
          : IHAL_IMU(model),
            acc_driver_(model, accel_dev, accel_int),
            gyro_driver_(model, gyro_dev, gyro_int)
        {};

    ~Bmi088_driver() = default;

    bool init(uint8_t unique_id, srimb::SRIMBTopic<RawAccData>& acc_topic, srimb::SRIMBTopic<RawGyroData>& gyro_topic, WorkQueue& wq) override {
        id_ = unique_id;

        bool gyro_status = gyro_driver_.init(unique_id, gyro_topic, wq);
        bool acc_status = acc_driver_.init(unique_id, acc_topic, wq);

        return (gyro_status && acc_status);
    };


private:
    Bmi088_acc_driver acc_driver_ ;
    Bmi088_gyro_driver gyro_driver_ ;
};
