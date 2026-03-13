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
void Logger::write_to_buffer(const void* data, unsigned int length)
{
    const uint8_t* byte_ptr = static_cast<const uint8_t*>(data);    // Casting a puntatore di tipo uint8
    for (unsigned int i = 0; i < length; i++)
    {
        _log_buffer[_head] = byte_ptr[i];
        _head = (_head + 1) % sizeof(_log_buffer);
    }
}

void Logger::log_imu()
{
    #ifdef CONFIG_LOG_IMU_ENABLED
        for (size_t i = 0; i < IMU_INSTANCES; i++)
        {
            ImuData imu_data;
            uint64_t timestamp;
            if (srimb_copy(topic_imu[i], *_srimb_subs_imu.at(i).get(), imu_data, timestamp))
            {
                LoggerMsgs<ImuData> imu_msg = {MAGIC_CHECK_BYTE, LOG_ID_IMU, timestamp, imu_data};

                _write_to_buffer(&imu_msg, sizeof(imu_msg));
            }
        }
    #endif
}

void Logger::log_gps()
{
    #ifdef CONFIG_LOG_GPS_ENABLED
        for (size_t i = 0; i < GPS_INSTANCES; i++)
        {
            GpsData gps_data;
            uint64_t timestamp;
            if (srimb_copy(topic_gps[i], *_srimb_subs_gps.at(i).get(), gps_data, timestamp))
            {
                LoggerMsgs<GpsData> gps_msg = {MAGIC_CHECK_BYTE, LOG_ID_GPS, timestamp, gps_data};
            
                _write_to_buffer(&gps_msg, sizeof(gps_msg));
            }
        }
    #endif
}

void Logger::log_tasks_trace(uint64_t frame_start, uint64_t hard_loop_finished, uint64_t soft_loop_started, uint64_t soft_loop_finished){
    
    #ifdef CONFIG_LOG_TASK_TRACE_FLAG

    #endif
    
};

/**------------------------------------------
 *              Public methods
 -------------------------------------------*/

void Logger::log(){
      
    _log_imu();
    _log_gps();

}