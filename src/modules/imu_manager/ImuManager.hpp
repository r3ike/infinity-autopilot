#pragma once

#include <zephyr/kernel.h>

#include "srimb.hpp"
#include "srimb_topic.hpp"
#include "imu_topic/imu_topic.hpp"


#include "HAL.hpp"



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

    void run();
private:
    HAL& _hal;


    void _publish_single_imu(uint8_t instance);
};


