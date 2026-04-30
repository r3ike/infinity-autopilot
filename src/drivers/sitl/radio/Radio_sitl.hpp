#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

#include "HAL.hpp"

class Radio_sitl : public IHAL_Radio
{
private:
    /* data */
public:
    Radio_sitl();
    ~Radio_sitl() = default;
};



