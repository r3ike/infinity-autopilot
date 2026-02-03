#include <drivers/imu/Imu.h>
#include "Imu.h"


Imu::Imu() {}
Imu::~Imu(){}

bool Imu::init() {
    _gyro_rate_calib = {0,0,0};

    bool res = connect();

    return res;
}

bool Imu::connect()
{
    bool success_flag = false;

    if (_bmi088.isConnection()) {
        _bmi088.initialize();
        success_flag = true;
    }

    return success_flag;
}

void Imu::calib(){
    _gyro_rate_calib = {0,0,0};
    for (int i = 0; i < IMU_CALIBRATION_STEP; i++)
    {
        _gyro_rate_calib = _gyro_rate_calib + getRawGyro();
    }

    _gyro_rate_calib = _gyro_rate_calib / IMU_CALIBRATION_STEP;
    
}

Vector3f Imu::getRawGyro(){
    float gyro[3];

    _bmi088.getGyroscope(&gyro[0], &gyro[1], &gyro[2]);

    return {gyro[0] - _gyro_rate_calib.x, gyro[1] - _gyro_rate_calib.y, gyro[2] - _gyro_rate_calib.z};
}

Vector3f Imu::getRawAccel(){
    float acc[3];

    _bmi088.getAcceleration(&acc[0], &acc[1], &acc[2]);

    return {acc[0], acc[1], acc[2]};
}

ImuData Imu::getRawImu()
{
    return {
        getRawGyro(),
        getRawAccel(),
        getImuTemp()
    };
}

int16_t Imu::getImuTemp()
{
    int16_t imuTemp = _bmi088.getTemperature();

    return imuTemp;
}
