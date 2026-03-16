#pragma once

#include "Logger.hpp"
#include "Module.hpp"

#include "PeriodicTask.hpp"
#include "Scheduler.hpp"

class LoggerTask : public IModule
{
private:
    Logger _logger;
    PeriodicTask _logger_task;
public:
    LoggerTask() : _logger_task([this]() { this->run(); }, fast_wq, K_MSEC(10)) {};
    ~LoggerTask() = default;

    void start() override {
        _logger_task.start();
    }

    void init() override {
        
    }

    void run() override {
        _logger.log();
    }
    

    void stop() override {
        _logger_task.stop();
    }
};




