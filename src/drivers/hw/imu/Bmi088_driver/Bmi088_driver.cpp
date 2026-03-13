
#include "Bmi088_driver.hpp"


Bmi088_driver::Bmi088_driver(const struct device *accel_dev, const struct device *gyro_dev):_accel_dev(accel_dev), _gyro_dev(gyro_dev) {
    _lpf_filter = std::make_unique<ImuLpfFilter>();
}

bool Bmi088_driver::init() {
    _gyro_rate_calib = {0,0,0};

    if (!device_is_ready(_accel_dev)) {
        //LOG_ERR("Device accelerometro non pronto");
        return false;
    }
    //LOG_INF("Accelerometro trovato: %s", accel_dev->name);

    if (!device_is_ready(_gyro_dev)) {
        //LOG_ERR("Device giroscopio non pronto");
        return false;
    }

    _lpf_filter->init_lpf_acc(CONFIG_IMU_SAMPLE_FREQUENCY, CONFIG_BMI088_ACCEL_LPF);
    _lpf_filter->init_lpf_gyro(CONFIG_IMU_SAMPLE_FREQUENCY, CONFIG_BMI088_GYRO_LPF);

    return true;
}



void Bmi088_driver::calib(){
    
    
}

Vector3f Bmi088_driver::get_raw_gyro(){
    struct sensor_value gyro[3];

    // --- Leggi giroscopio ---
    if (sensor_sample_fetch(_gyro_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione giroscopio");
        return {0,0,0};
    }

    sensor_channel_get(_gyro_dev, SENSOR_CHAN_GYRO_XYZ, gyro);

    return {
        sensor_value_to_double(&gyro[0]),
        sensor_value_to_double(&gyro[1]),
        sensor_value_to_double(&gyro[2])
    };
}

Vector3f Bmi088_driver::get_raw_accel(){
    struct sensor_value accel[3];

    // --- Leggi accelerometro ---
    if (sensor_sample_fetch(_accel_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione accelerometro");
        return {0,0,0};
    }

    sensor_channel_get(_accel_dev, SENSOR_CHAN_ACCEL_XYZ, accel);


    return {
        sensor_value_to_double(&accel[0]),
        sensor_value_to_double(&accel[1]),
        sensor_value_to_double(&accel[2])
    };
}

ImuData Bmi088_driver::get_imu()
{
    ImuData data = {
        get_raw_gyro(),
        get_raw_accel(),
        {0,0,0},
        {0,0,0},
        _get_imu_temp()
    };

    _lpf_filter->apply(data);

    return data;
}

double Bmi088_driver::_get_imu_temp()
{
    struct sensor_value temp_value;

    // 1. Effettua una lettura del sensore (il giroscopio, che include il die temp)
    if (sensor_sample_fetch(_gyro_dev) < 0) {
        return 0.0;
    }

    // 2. Ottieni il valore del canale temperatura
    if (sensor_channel_get(_gyro_dev, SENSOR_CHAN_DIE_TEMP, &temp_value) < 0) {
        return 0.0;
    }

    // 3. Converti e logga
    double temp_celsius = sensor_value_to_double(&temp_value);
    return temp_celsius;
}
