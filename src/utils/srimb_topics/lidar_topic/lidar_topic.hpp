#pragma once
#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include "srimb_topic.hpp"
#include "LidarData.hpp"
#include "HAL_configs.hpp"


extern SRIMB_Topic<LidarData> topic_lidar[LIDAR_INSTANCES];
