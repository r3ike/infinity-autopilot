#pragma once

#include "modules/SRIMB/srimb.hpp"
#include "modules/SRIMB/srimb_topic.hpp"
#include "modules/SRIMB/topics/imu_topic.hpp"

#include "hal/HAL.hpp"

#include <filters/ImuLpfFilter.hpp>

/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 * sarà il modulo addetto a gestire il Fault Detection and Isolation quando verrà implementato.
 */

class ImuManager
{
public:
    ImuManager(HAL& hal) : _hal(hal){};
    ~ImuManager() = default;

    void init();

    void publish();
private:
    HAL& _hal;

    ImuLpfFilter _imu_lpf_filter;

    void _publish_single_imu(uint8_t instance);
};


