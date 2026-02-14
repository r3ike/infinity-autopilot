#pragma once
#include "modules/SRIMB/srimb_topic.hpp"
#include <cstdint>
#include <./utils/Vector3f.h>
#include <config/parameters.h>


struct LidarData
{
    int16_t altitude;       //
    int16_t tfFlux;         // Qualità del segnale di ritorno
    int16_t tfTemp;         //temperatura del sensore
    uint8_t lidar_id = 0;
};

extern SRIMB_Topic<LidarData> topic_lidar[IMU_INSTANCES];
