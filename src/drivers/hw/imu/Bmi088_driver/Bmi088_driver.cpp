#include <hal/teensy/drivers/imu/Bmi088_driver/Bmi088_driver.hpp>
#include "Bmi088_driver.hpp"


Bmi088_driver::Bmi088_driver() {}

bool Bmi088_driver::init() {
    _gyro_rate_calib = {0,0,0};

    if (!device_is_ready(accel_dev)) {
        //LOG_ERR("Device accelerometro non pronto");
        return false;
    }
    //LOG_INF("Accelerometro trovato: %s", accel_dev->name);

    if (!device_is_ready(gyro_dev)) {
        //LOG_ERR("Device giroscopio non pronto");
        return false;
    }

    return true;
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
    struct sensor_value accel_x, accel_y, accel_z;

    // --- Leggi accelerometro ---
    if (sensor_sample_fetch(accel_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione accelerometro");
        return;
    }

    sensor_channel_get(accel_dev, SENSOR_CHAN_ACCEL_X, &accel_x);
    sensor_channel_get(accel_dev, SENSOR_CHAN_ACCEL_Y, &accel_y);
    sensor_channel_get(accel_dev, SENSOR_CHAN_ACCEL_Z, &accel_z);


    return {
        sensor_value_to_double(&accel_x),
        sensor_value_to_double(&accel_y),
        sensor_value_to_double(&accel_z)
    };
}

ImuData Bmi088_driver::getRawImu()
{
    struct sensor_value gyro_x, gyro_y, gyro_z;

    // --- Leggi giroscopio ---
    if (sensor_sample_fetch(gyro_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione giroscopio");
        return;
    }

    sensor_channel_get(gyro_dev, SENSOR_CHAN_GYRO_X, &gyro_x);
    sensor_channel_get(gyro_dev, SENSOR_CHAN_GYRO_Y, &gyro_y);
    sensor_channel_get(gyro_dev, SENSOR_CHAN_GYRO_Z, &gyro_z);

    return {
        getRawGyro(),
        getRawAccel(),
        {0,0,0},
        {0,0,0},
        _getImuTemp()
    };
}

double Bmi088_driver::_getImuTemp()
{
    struct sensor_value temp_value;

    // 1. Effettua una lettura del sensore (il giroscopio, che include il die temp)
    if (sensor_sample_fetch(gyro_dev) < 0) {
        LOG_ERR("Fallita acquisizione campione per temperatura");
        return;
    }

    // 2. Ottieni il valore del canale temperatura
    if (sensor_channel_get(gyro_dev, SENSOR_CHAN_DIE_TEMP, &temp_value) < 0) {
        LOG_ERR("Impossibile leggere il canale temperatura");
        return;
    }

    // 3. Converti e logga
    double temp_celsius = sensor_value_to_double(&temp_value);
    return temp_celsius
}
