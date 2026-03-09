#pragma once

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include "srimb_topic.hpp"
#include <cstdint>
#include "Vector3f.h"
#include "HAL.hpp"


struct GpsData
{
    double latitude;
    double longitude;
    double altitude;
    double course;
    double groundSpeed;

    double pos_e;       //ENU est position
    double pos_n;       //ENU north position
    double pos_u;       //ENU up position

    double vel_e;       //ENU est velocity
    double vel_n;       //ENU north velocity
    double vel_u;       //ENU up veloup

    uint32_t sat;
    double hdop;
    uint8_t time[7];        //GG MM YY H M S centisecond
    uint32_t fixAge;
    
    uint8_t gps_id = 0;
};

extern SRIMB_Topic<GpsData> topic_gps[GPS_INSTANCES];
