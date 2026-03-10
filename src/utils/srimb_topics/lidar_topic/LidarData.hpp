#pragma once
#include <cstdint>
#include "Vector3f.h"


struct LidarData
{
    int16_t altitude;       //
    int16_t tfFlux;         // Qualità del segnale di ritorno
    int16_t tfTemp;         //temperatura del sensore
    uint8_t lidar_id = 0;
};