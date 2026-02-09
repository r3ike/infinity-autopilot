#pragma once

#include "modules/SRIMB/srimb.hpp"
#include "modules/SRIMB/topics/imu_topic.hpp"

#include "hal/HAL.hpp"

/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 * sarà il modulo addetto a gestire il Fault Detection and Isolation quando verrà implementato.
 */

class ImuManager
{
public:
    ImuManager();
    ~ImuManager() = default;
private:

};


