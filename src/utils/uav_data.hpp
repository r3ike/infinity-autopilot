#pragma once
#include <cstdint>
#include <./utils/Vector3f.h>

struct GpsData
{
    double latitude;
    double longitude;
    double altitude;
    double course;
    double groundSpeed;
    uint32_t sat;
    double hdop;
    uint8_t time[7];        //GG MM YY H M S centisecond
    uint32_t fixAge;
};

struct ImuData
{
    Vector3f gyro;
    Vector3f acc;
    int16_t temp;
};

struct LidarData
{
    int16_t altitude;       //
    int16_t tfFlux;         // Qualità del segnale di ritorno
    int16_t tfTemp;         //temperatura del sensore
};
