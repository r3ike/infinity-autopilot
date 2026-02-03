#include <utils/Logger/Logger.hpp>
#include "Logger.hpp"


Logger::Logger(){}

Logger &Logger::getInstance()
{
    // Questa variabile 'instance' viene creata solo la prima volta 
    // che chiami getInstance(). Le volte successive viene restituita quella già esistente.
    static Logger instance; 
    return instance;
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
        LogImuMsg imu_msg = {0xA5, LOG_ID_IMU, timestamp, imu_raw, imu_filtered};

        writeToBuffer(&imu_msg, sizeof(LogImuMsg));
    }
    
}