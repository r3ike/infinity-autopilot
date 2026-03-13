#pragma once 

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <sys/util.h>

#include "HAL.hpp"
#ifdef CONFIG_LOGGER_ENABLED
#include "Logger.hpp"
#endif

#include "ImuManagerTask.hpp"

#include "Scheduler.hpp"

class FlightController
{
private:
    HAL _hal;

    ImuManagerTask _imu_manager_task;

    
public:
    
    FlightController() : _imu_manager_task(_hal) {};
    ~FlightController() = default;
    
    void init();

    void start():


};


