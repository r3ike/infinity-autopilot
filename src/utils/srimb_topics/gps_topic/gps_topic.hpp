#pragma once

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include "srimb_topic.hpp"
#include "GpsData.hpp"
#include "HAL.hpp"

extern SRIMB_Topic<GpsData> topic_gps[GPS_INSTANCES];
