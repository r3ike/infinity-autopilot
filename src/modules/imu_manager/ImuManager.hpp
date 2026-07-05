#pragma once

#include <zephyr/kernel.h>

#include "srimb.hpp"
#include "srimb_topic.hpp"
#include "uav_types.hpp"


#include "HAL.hpp"



/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 */

class ImuManager
{
public:
    ImuManager(HAL& hal) : _hal(hal){};

    ~ImuManager() = default;

    void init();

    void run();
private:

    void publish_single_imu(uint8_t instance);
};


