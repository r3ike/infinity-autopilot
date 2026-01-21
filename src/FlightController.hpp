#pragma once 
#include <config/parameters.h>

#include <hal/HAL.hpp>

#ifdef HAL_TEENSY
    #include <Arduino_FreeRTOS.h>
#elif defined(HAL_SITL)
    
#endif

//HAL* hal;

class FlightController
{
private:
    HAL hal;
public:
    FlightController(/* args */);
    ~FlightController();
    
    void init();
    static void TaskAttitudeEstimation(void *pvParameters);

};


