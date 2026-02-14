#pragma once
//#include <utils/uav_data.hpp>

/**
 * Definition of log msg types
 */
#pragma pack(push, 1) // 1 significa allineamento al singolo byte

enum LogMsgID
{
    LOG_ID_SYS_INFO,    // Log di sistema
    LOG_ID_TASK_TRACE,  // Log  per lo scheduler
    LOG_ID_IMU,
    LOG_ID_GPS,
    LOG_ID_EKF_STATE,
    LOG_ID_EKF_INNOV,
    LOG_ID_EKF_COV,
    LOG_ID_CONTROL,
    LOG_ID_STATE_CHANGES
};

// Log per la imu
template<typename T>
struct LoggerMsgs
{
    uint8_t sync;                   //BYTE PER IDENTIFICARE L'INIZIO DEL PACCHETTO
    uint8_t msg_type;
    uint32_t timestamp;
    T data;
};

/*
struct LogImuMsg
{
    uint8_t sync;                   //BYTE PER IDENTIFICARE L'INIZIO DEL PACCHETTO
    uint8_t msg_type;
    uint32_t timestamp;
    ImuData imu_data_raw;            //RAW
    ImuData imu_data_filtered;       //FILTERED
};
*/
/*
struct LogTasksTraceMsg
{
    uint8_t sync;
    uint8_t msg_type;
    uint32_t frame_start;
    uint32_t hard_loop_finished;
    uint32_t soft_loop_started;
    uint32_t soft_loop_finished;
};
*/

#pragma pack(pop)
