#include <utils/Logger/Logger.hpp>
#include "Logger.hpp"

Logger::Logger()
{
}
void Logger::logImu(uint32_t timestamp, ImuData imu_raw, ImuData imu_filtered)
{
    if (LOG_IMU_FLAG)
    {
        LogImuMsg imu_msg = {0xA5, LOG_ID_IMU, timestamp, imu_raw, imu_filtered};

        unsigned int imu_msg_length = sizeof(LogImuMsg);

        memcpy(&_log_buffer[_head], &imu_msg, imu_msg_length);

        _head += imu_msg_length;
    }
    
}