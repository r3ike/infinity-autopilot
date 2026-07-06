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

struct __attribute__((packed)) LogMsgHeader {
    uint8_t  sync;        // MAGIC_CHECK_BYTE
    uint8_t  msg_id;       // LogMsgId
    uint16_t payload_size; // sizeof(T) — utile per il parser
    uint64_t timestamp_us;
};

// Log per la imu
template<typename T>
struct LoggerMsgs
{
    LogMsgHeader header;
    T payload;
};


#pragma pack(pop)
