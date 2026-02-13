#include <hal/teensy/drivers/imu/Bmi088_driver/Bmi088_driver.hpp>
#include "Bmi088_driver.hpp"


Bmi088_driver::Bmi088_driver() {}
Bmi088_driver::~Bmi088_driver(){}

bool Bmi088_driver::init() {
    _gyro_rate_calib = {0,0,0};

    bool res = _connect();

    return res;
}

bool Bmi088_driver::_connect()
{
    bool success_flag = false;

    if (_bmi088.isConnection()) {
        _bmi088.initialize();
        success_flag = true;
    }

    return success_flag;
}

void Bmi088_driver::calib(){
    _gyro_rate_calib = {0,0,0};
    for (int i = 0; i < IMU_CALIBRATION_STEP; i++)
    {
        _gyro_rate_calib = _gyro_rate_calib + getRawGyro();
    }

    _gyro_rate_calib = _gyro_rate_calib / IMU_CALIBRATION_STEP;
    
}

Vector3f Bmi088_driver::getRawGyro(){
    float gyro[3];

    _bmi088.getGyroscope(&gyro[0], &gyro[1], &gyro[2]);

    return {gyro[0] - _gyro_rate_calib.x, gyro[1] - _gyro_rate_calib.y, gyro[2] - _gyro_rate_calib.z};
}

Vector3f Bmi088_driver::getRawAccel(){
    float acc[3];

    _bmi088.getAcceleration(&acc[0], &acc[1], &acc[2]);

    return {acc[0], acc[1], acc[2]};
}

ImuData Bmi088_driver::getRawImu()
{
    return {
        getRawGyro(),
        getRawAccel(),
        {0,0,0},
        {0,0,0},
        _getImuTemp()
    };
}

int16_t Bmi088_driver::_getImuTemp()
{
    int16_t imuTemp = _bmi088.getTemperature();

    return imuTemp;
}
