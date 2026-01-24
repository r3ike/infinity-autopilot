#pragma once 
#include <config/parameters.h>
#include <utils/Convertions.hpp>
#include <hal/HAL.hpp>
#include <scheduler/TTCEScheduler.hpp>
#include <utils/uav_data.hpp>


//HAL* hal;

class FlightController
{
private:
    HAL hal;

public:
    FlightController(/* args */);
    ~FlightController();
    
    void init();
    
    void run();

};


