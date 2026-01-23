#pragma once 
#include <config/parameters.h>

#include <hal/HAL.hpp>
#include <scheduler/TTCEScheduler.hpp>


//HAL* hal;

class FlightController
{
private:
    HAL hal;
    TTCEScheduler* scheduler;

    void hardLoopTasks();
public:
    FlightController(/* args */);
    ~FlightController();
    
    void init();
    
    void run();

};


