#pragma once 
#include <config/parameters.h>
#include <Convertions.hpp>

#include <HAL.hpp>

#include <modules/Logger/Logger.hpp>
#include <modules/ImuManager/ImuManager.hpp>

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


