#pragma once
#include <cstdint>
#include "Vector3f.h"

struct RawImuData {

};

struct RawGyroData {
    uint64_t timestamp;

    uint8_t id;
    
    float x;
    float y;
    float z;

    float temperature;
};

struct RawAccData{
    uint64_t timestamp;

    uint8_t id;
    
    float x;
    float y;
    float z;

};

struct ImuData {
    Vector3f raw_acc;
    Vector3f raw_gyro;

    //Imu dopo essere stato pre-filtrato => LPF, notch filter, temperature compensation, sculling e coning compensation
    Vector3f filtered_acc;          
    Vector3f filtered_gyro;

    double temp;
    uint8_t imu_id = 0;
    //Da aggiungere scoring e stato sensore e capire se mettere quaternioni qua(quindi se fare la conversione nel driver o nel imuManager)
};

struct GpsData
{
    double latitude;
    double longitude;
    double altitude;
    double course;
    double groundSpeed;

    double pos_e;       //ENU est position
    double pos_n;       //ENU north position
    double pos_u;       //ENU up position

    double vel_e;       //ENU est velocity
    double vel_n;       //ENU north velocity
    double vel_u;       //ENU up veloup

    uint32_t sat;
    double hdop;
    uint8_t time[7];        //GG MM YY H M S centisecond
    uint32_t fixAge;
    
    uint8_t gps_id = 0;
};

struct LidarData
{
    int16_t altitude;       //
    int16_t tfFlux;         // Qualità del segnale di ritorno
    int16_t tfTemp;         //temperatura del sensore
    uint8_t lidar_id = 0;
};

struct BaroData {
    uint8_t baro_id = 0;
};

struct MagData {
    uint8_t mag_id = 0;
};