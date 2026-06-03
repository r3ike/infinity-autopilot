#pragma once 

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>

#include "SRIMB.hpp"
#include "HAL.hpp"

#ifdef CONFIG_LOGGER_ENABLED
#include "LoggerTask.hpp"
#endif

#include "ImuManagerTask.hpp"

#include "Scheduler.hpp"

class FlightController
{
private:
    srimb::SRIMB<float> temp_topic;

    HAL _hal;

    ImuManagerTask _imu_manager_task;
    LoggerTask _logger_task;

    
public:
    
    FlightController() : _imu_manager_task(_hal) {};
    ~FlightController() = default;
    
    void init();

    void start();


};


