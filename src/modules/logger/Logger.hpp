#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <memory>

#include <zephyr/kernel.h>
#include "ringbuffer/RingBuffer.hpp"

#include "LoggerMsgs.hpp"
#include "HAL_configs.hpp"
#include "SRIMB.hpp"
#include "uav_types.hpp"

#define MAGIC_CHECK_BYTE 0xA5

/**
 * La classe Logger si occupa di salvare in un buffer circolare in RAM i messagi di log (definiti in LoggerMsgs.hpp).
 * 
 * La dimensione può essere modificata dal file parameters.h.
 * Tale buffer sarà scritto su sd ogni volta che il driver della SD ritiene che sia necessario.
 * 
 */
class Logger
{
private:
    /**
     * TODO:
     *  - capire come fare il write_to_buffer, se mantenere questo metodo o usare memcpy
     *  - scrivere drain
     */

    uint8_t _log_buffer[CONFIG_LOGGER_BUFFER_SIZE * 1024];

    RingBuffer<uint8_t> log_buffer_;


    void write_to_buffer(const void* data, size_t length);

    void log_imu();

    void log_gps();
    
    void log_tasks_trace(uint64_t frame_start, uint64_t hard_loop_finished, uint64_t soft_loop_started, uint64_t soft_loop_finished);
public:
    Logger() : log_buffer_(CONFIG_LOGGER_BUFFER_SIZE * 1024);
    ~Logger() = default;


    void log();

    /** 
     *  metodo chiamato dal writer task (sd manager) per drenare il buffer del logger 
     * ritorna quanti byte ha copiato
     */
    uint32_t drain(uint8_t *buf, uint32_t max_len);

    
};


