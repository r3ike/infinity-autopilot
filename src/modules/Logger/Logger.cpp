#include <modules/Logger/Logger.hpp>
#include "Logger.hpp"


Logger::Logger(){
    for (size_t i = 0; i < IMU_INSTANCES; i++)  _srimb_subs_imu[i] = std::make_unique<SRIMB_Sub>();
    for (size_t i = 0; i < GPS_INSTANCES; i++)  _srimb_subs_gps[i] = std::make_unique<SRIMB_Sub>();
    
}



/**------------------------------------------
 *              Private methods
 -------------------------------------------*/

/**
 * Funzione per scrivere byte per byte un log_msg nel buffer dei log
 * 
 * void* data è un puntatore generico e universale,
 *  così che possiamo passare qualsiasi tipo di struct come parametro
 */
void Logger::_write_to_buffer(const void* data, unsigned int length)
{
    const uint8_t* byte_ptr = static_cast<const uint8_t*>(data);    // Casting a puntatore di tipo uint8
    for (unsigned int i = 0; i < length; i++)
    {
        _log_buffer[_head] = byte_ptr[i];
        _head = (_head + 1) % sizeof(_log_buffer);
    }
}

void Logger::_log_imu(uint32_t timestamp, ImuData imu_data)
{
    if (LOG_IMU_FLAG)
    {
        LoggerMsgs<ImuData> imu_msg = {MAGIC_CHECK_BYTE, LOG_ID_IMU, timestamp, imu_data};

        _write_to_buffer(&imu_msg, sizeof(imu_msg));
    }
    
}

void Logger::_log_tasks_trace(uint32_t frame_start, uint32_t hard_loop_finished, uint32_t soft_loop_started, uint32_t soft_loop_finished){
    if (LOG_TASK_TRACE_FLAG)
    {
    }
    
};

/**------------------------------------------
 *              Public methods
 -------------------------------------------*/

void Logger::log(){
    
    
    
}