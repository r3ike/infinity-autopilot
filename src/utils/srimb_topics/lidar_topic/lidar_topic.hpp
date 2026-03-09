#pragma once
#include <cstdint>

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include "srimb_topic.hpp"
#include "Vector3f.h"
#include "HAL.hpp"


struct LidarData
{
    int16_t altitude;       //
    int16_t tfFlux;         // Qualità del segnale di ritorno
    int16_t tfTemp;         //temperatura del sensore
    uint8_t lidar_id = 0;
};

extern SRIMB_Topic<LidarData> topic_lidar[LIDAR_INSTANCES];
