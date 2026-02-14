#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <memory>

#include <modules/Logger/LoggerMsgs.hpp>
#include <config/parameters.h>

#include "modules/SRIMB/srimb.hpp"
#include "utils/srimb_topics/imu_topic.hpp"
#include "utils/srimb_topics/gps_topic.hpp"

#define MAGIC_CHECK_BYTE 0xA5

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

    uint8_t _log_buffer[BUFFER_SIZE_KB * 1024];
    volatile uint32_t _head = 0; // Dove scrive il Producer
    volatile uint32_t _tail = 0; // Dove legge il Consumer (SD)

    std::array<std::unique_ptr<SRIMB_Sub>, IMU_INSTANCES> _srimb_subs_imu;
    std::array<std::unique_ptr<SRIMB_Sub>, GPS_INSTANCES> _srimb_subs_gps;
    

    void _write_to_buffer(const void* data, unsigned int length);

    void _log_imu();

    void _log_gps();
    
    void _log_tasks_trace(uint64_t frame_start, uint64_t hard_loop_finished, uint64_t soft_loop_started, uint64_t soft_loop_finished);
public:
    Logger();
    ~Logger() = default;

    void log();

    
};


