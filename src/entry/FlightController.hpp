#pragma once 

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>

#include "HAL.hpp"
#ifdef CONFIG_LOGGER_ENABLED
#include "Logger.hpp"
#endif

#include "ImuManager.hpp"

class FlightController
{
private:
    HAL _hal;
    Logger _logger;
    ImuManager _imu_manager;

    
public:
    
    FlightController() : _imu_manager(_hal) {};
    ~FlightController() = default;
    
    void init();


};


