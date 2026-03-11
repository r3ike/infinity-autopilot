#pragma once 

#include "Convertions.hpp"
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

    
    FlightController() : _imu_manager(_hal) {};
    ~FlightController() = default;

    void hardLoop();

public:

    
    void init();


};


