#pragma once

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>

/**
 * Macro che controlla se ci sono nodi con compat == ad un compatible di una imu
 * e in caso positivo lancia la macro di registrazione specifica
 */
#define REGISTER_IMU_IF_COMPATIBLE(compat, driver_class)        \
    COND_CODE_1(DT_HAS_COMPAT(compat),                          \
            (DT_INST_FOREACH_STATUS_OKAY(                       \
                REGISTER_##driver_class##_INST                  \
            )), ())                                             \


/**
 * ----------------------------------------------------
 * Macro specifiche per la registrazione dei singoli 
 * modelli di imu
 * ----------------------------------------------------
 */


 #define REGISTER_BMI088_INST(inst)                                                                 \
    do{                                                                                             \
       const struct device *accel_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), accel));     \
       const struct device *gyro_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), gyro));       \ 
       register_imu(std::make_unique<Bmi088_driver>(accel_device, gyro_device));                    \                             
    }while(0)                                                                                       \


#define REGISTER_ALL_IMUS()                                                                 \
    REGISTER_IMU_IF_COMPATIBLE(infinity_autopilot_bmi088_imu, BMI088)                       \


    
    
#ifdef CONFIG_TARGET_TEENSY41
    
#define NUM_BMI088_INSTANCES DT_NUM_INST_STATUS_OKAY(infinity_autopilot_bmi088_imu)
#define NUM_ICM42688P_INSTANCES DT_NUM_INST_STATUS_OKAY(infinity_autopilot_icm42688p_imu)


#define IMU_INSTANCES NUM_BMI088_INSTANCES + NUM_ICM42688P_INSTANCES
#define GPS_INSTANCES 1
#define LIDAR_INSTANCES 1
#define BARO_INSTANCES 1
#define MAG_INSTANCES 1

#else

#define IMU_INSTANCES 1
#define GPS_INSTANCES 1
#define LIDAR_INSTANCES 1
#define BARO_INSTANCES 1
#define MAG_INSTANCES 1

#endif