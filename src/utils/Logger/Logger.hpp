#pragma once
#include <utils/Logger/LoggerMsgs.hpp>
#include <config/parameters.h>

class Logger
{
private:
    uint8_t _log_buffer[BUFFER_SIZE_KB * 1024];
    volatile uint32_t _head = 0; // Dove scrive il Producer
    volatile uint32_t _tail = 0; // Dove legge il Consumer (SD)

    void writeToBuffer(const void* data, unsigned int length);

public:
    Logger();
    ~Logger() = default;

    void logImu(uint32_t timestamp, ImuData imu_raw, ImuData imu_filtered);
    void logTask();
    void LogTasksTrace();
    
};


