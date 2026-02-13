#include <modules/Logger/Logger.hpp>
#include "Logger.hpp"


Logger::Logger(){}



/**------------------------------------------
 *              Private methods
 -------------------------------------------*/

/**
 * Funzione per scrivere byte per byte un log_msg nel buffer dei log
 * 
 * void* data è un puntatore generico e universale,
 *  così che possiamo passare qualsiasi tipo di struct come parametro
 */
void Logger::writeToBuffer(const void* data, unsigned int length)
{
    const uint8_t* byte_ptr = static_cast<const uint8_t*>(data);    // Casting a puntatore di tipo uint8
    for (unsigned int i = 0; i < length; i++)
    {
        _log_buffer[_head] = byte_ptr[i];
        _head = (_head + 1) % sizeof(_log_buffer);
    }
}


/**------------------------------------------
 *              Public methods
 -------------------------------------------*/


void Logger::logImu(uint32_t timestamp, ImuData imu_raw, ImuData imu_filtered)
{
    if (LOG_IMU_FLAG)
    {
        LogImuMsg imu_msg = {MAGIC_CHECK_BYTE, LOG_ID_IMU, timestamp, imu_raw, imu_filtered};

        writeToBuffer(&imu_msg, sizeof(LogImuMsg));
    }
    
}

void Logger::LogTasksTrace(uint32_t frame_start, uint32_t hard_loop_finished, uint32_t soft_loop_started, uint32_t soft_loop_finished){
    if (LOG_TASK_TRACE_FLAG)
    {
        LogTasksTraceMsg tasks_msg = {MAGIC_CHECK_BYTE, LOG_ID_TASK_TRACE, frame_start, hard_loop_finished, soft_loop_started, soft_loop_finished};

        writeToBuffer(&tasks_msg, sizeof(LogTasksTraceMsg));
    }
    
};