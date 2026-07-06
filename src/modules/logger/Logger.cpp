#include "Logger.hpp"


Logger::Logger(){
    
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
void Logger::write_to_buffer(const void* data, size_t length)
{
    const uint8_t* src = reinterpret_cast<const uint8_t*>(&msg);
    log_buffer_.push_batch(src, length);
}

void Logger::log_imu()
{
#ifdef CONFIG_LOG_IMU_ENABLED
    for (size_t i = 0; i < IMU_INSTANCES; i++)
    {
        ImuData imu_data;
        uint64_t timestamp;
        if (srimb_copy(topic_imu[i], _srimb_subs_imu[i], imu_data, timestamp))
        {
            LoggerMsgs<ImuData> imu_msg = {
                .header = {
                    .magic = MAGIC_CHECK_BYTE,
                    .msg_id = LogMsgID::LOG_ID_IMU,
                    .payload_size = sizeof(ImuData),
                    .timestamp_us = timestamp,
                },
                .payload = imu_data;
            };
            write_to_buffer(&imu_msg, sizeof(imu_msg));
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
            if (srimb_copy(topic_gps[i], _srimb_subs_gps[i], gps_data, timestamp))
            {
                LoggerMsgs<GpsData> gps_msg = {MAGIC_CHECK_BYTE, LOG_ID_GPS, timestamp, gps_data};
            
                write_to_buffer(&gps_msg, sizeof(gps_msg));
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
      
    log_imu();
    log_gps();

}