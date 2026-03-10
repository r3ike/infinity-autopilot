#pragma once
#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include "srimb_topic.hpp"
#include "ImuData.hpp"
#include "HAL.hpp"


extern SRIMB_Topic<ImuData> topic_imu[IMU_INSTANCES];
