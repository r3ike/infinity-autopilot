#pragma once

#include "modules/SRIMB/srimb.hpp"
#include "modules/SRIMB/srimb_topic.hpp"
#include "utils/srimb_topics/imu_topic.hpp"
#include <config/parameters.h>

#include "hal/HAL.hpp"

#include <filters/ImuLpfFilter.hpp>

/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 * sarà il modulo addetto a gestire il Fault Detection and Isolation quando verrà implementato.
 */

class ImuManager
{
public:
    ImuManager(HAL& hal) : _hal(hal){
        for (size_t i = 0; i < IMU_INSTANCES; i++)
        {
            _imu_lpf_filters[i] = std::make_unique<ImuLpfFilter>();
        }
        
        
    };
    ~ImuManager() = default;

    void init();

    void run();
private:
    HAL& _hal;

    std::array<std::unique_ptr<ImuLpfFilter>, IMU_INSTANCES> _imu_lpf_filters;

    void _publish_single_imu(uint8_t instance);
};


