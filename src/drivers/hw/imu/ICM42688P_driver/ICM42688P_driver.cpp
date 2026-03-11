
#include "ICM42688P_driver.hpp"


ICM42688P_driver::ICM42688P_driver(const struct device *imu_dev):_imu_dev(imu_dev){
    _lpf_filter = std::make_unique<ImuLpfFilter>();
}

bool ICM42688P_driver::init() {
    _gyro_rate_calib = {0,0,0};

    if (!device_is_ready(_imu_dev)) {
        //LOG_ERR("Device accelerometro non pronto");
        return false;
    }
    //LOG_INF("Accelerometro trovato: %s", accel_dev->name);


    _lpf_filter->init_lpf_acc(CONFIG_IMU_SAMPLE_FREQUENCY, CONFIG_ICM42688P_ACCEL_LPF);
    _lpf_filter->init_lpf_gyro(CONFIG_IMU_SAMPLE_FREQUENCY, CONFIG_ICM42688P_GYRO_LPF);

    return true;
}



void ICM42688P_driver::calib(){
    
    
}

Vector3f ICM42688P_driver::getRawGyro(){
    struct sensor_value gyro[3];

    // --- Leggi giroscopio ---
    if (sensor_sample_fetch(_gyro_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione giroscopio");
        return {0,0,0};
    }

    sensor_channel_get(_imu_dev, SENSOR_CHAN_GYRO_XYZ, gyro);

    return {
        sensor_value_to_double(&gyro[0]),
        sensor_value_to_double(&gyro[1]),
        sensor_value_to_double(&gyro[2])
    };
}

Vector3f ICM42688P_driver::getRawAccel(){
    struct sensor_value accel[3];

    // --- Leggi giroscopio ---
    if (sensor_sample_fetch(_imu_dev) < 0) {
        // LOG_ERR("Fallita acquisizione campione giroscopio");
        return {0,0,0};
    }

    sensor_channel_get(_imu_dev, SENSOR_CHAN_ACCEL_XYZ, accel);

    return {
        sensor_value_to_double(&accel[0]),
        sensor_value_to_double(&accel[1]),
        sensor_value_to_double(&accel[2])
    };
}

ImuData ICM42688P_driver::getRawImu()
{
    ImuData data = {
        getRawGyro(),
        getRawAccel(),
        {0,0,0},
        {0,0,0},
        _getImuTemp()
    };

    _lpf_filter->apply(data);

    return data;
}

double ICM42688P_driver::_getImuTemp()
{
    struct sensor_value temp_value;

    // 1. Effettua una lettura del sensore (il giroscopio, che include il die temp)
    if (sensor_sample_fetch(_imu_dev) < 0) {
        return 0.0;
    }

    // 2. Ottieni il valore del canale temperatura
    if (sensor_channel_get(_imu_dev, SENSOR_CHAN_DIE_TEMP, &temp_value) < 0) {
        return 0.0;
    }

    // 3. Converti e logga
    double temp_celsius = sensor_value_to_double(&temp_value);
    return temp_celsius;
}
