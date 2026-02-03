#pragma once
#include <utils/Logger/LoggerMsgs.hpp>
#include <config/parameters.h>


/**
 * La classe Logger si occupa di salvare in un buffer circolare in RAM i messagi di log (definiti in LoggerMsgs.hpp).
 * 
 * La dimensione può essere modificata dal file parameters.h.
 * Tale buffer sarà scritto su sd ogni volta che il driver della SD ritiene che sia necessario.
 * 
 * La classe Logger è implementata attraverso un Meyers Singleton.
 */
class Logger
{
private:
    Logger();
    ~Logger() = default;


    uint8_t _log_buffer[BUFFER_SIZE_KB * 1024];
    volatile uint32_t _head = 0; // Dove scrive il Producer
    volatile uint32_t _tail = 0; // Dove legge il Consumer (SD)

    void writeToBuffer(const void* data, unsigned int length);

public:
    static Logger& getInstance();

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;


    void logImu(uint32_t timestamp, ImuData imu_raw, ImuData imu_filtered);
    
    void LogTasksTrace();
    
};


