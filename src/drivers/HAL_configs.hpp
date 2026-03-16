#pragma once

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>



#ifdef CONFIG_TARGET_TEENSY41

// BN280
#ifdef CONFIG_BN280_DRIVER_ENABLED
#define BN280_INSTANCES CONFIG_BN280_NUM_INSTANCES
#else
#define BN280_INSTANCES 0
#endif

// BMI088
#ifdef CONFIG_BMI088_DRIVER_ENABLED
#define BMI088_INSTANCES CONFIG_BMI088_NUM_INSTANCES
#else
#define BMI088_INSTANCES 0
#endif

// BMI088
#ifdef CONFIG_ICM42688P_DRIVER_ENABLED
#define ICM42688P_INSTANCES CONFIG_ICM42688P_NUM_INSTANCES
#else
#define ICM42688P_INSTANCES 0
#endif

#define IMU_INSTANCES (BMI088_INSTANCES + ICM42688P_INSTANCES)
#define GPS_INSTANCES (BN280_INSTANCES + 0)
#define MAG_INSTANCES 1
#define LIDAR_INSTANCES 1
#define BARO_INSTANCES 1

// Configurazioni per il multi-instance del Bmi088
#ifdef CONFIG_BMI088_DRIVER_ENABLED 

#define BMI088_ACCEL_0 DT_NODELABEL(bmi088_accel_0)
#define BMI088_GYRO_0  DT_NODELABEL(bmi088_gyro_0)
#define BMI088_ACCEL_1 DT_NODELABEL(bmi088_accel_1)
#define BMI088_GYRO_1  DT_NODELABEL(bmi088_gyro_1)
#define BMI088_ACCEL_2 DT_NODELABEL(bmi088_accel_2)
#define BMI088_GYRO_2  DT_NODELABEL(bmi088_gyro_2)

// Istanza 0
#if (CONFIG_BMI088_NUM_INSTANCES >=1) && !DT_NODE_EXISTS(BMI088_ACCEL_0)
#error "BMI088 instance 0 accelerometer node not found in device tree"
#endif
#if (CONFIG_BMI088_NUM_INSTANCES >=1) && !DT_NODE_EXISTS(BMI088_GYRO_0)
#error "BMI088 instance 0 gyroscope node not found in device tree"
#endif
// Istanza 1
#if (CONFIG_BMI088_NUM_INSTANCES >=2) && !DT_NODE_EXISTS(BMI088_ACCEL_1)
#error "BMI088 instance 1 accelerometer node not found in device tree"
#endif
#if (CONFIG_BMI088_NUM_INSTANCES >=2) && !DT_NODE_EXISTS(BMI088_GYRO_1)
#error "BMI088 instance 1 gyroscope node not found in device tree"
#endif
// Istanza 2
#if (CONFIG_BMI088_NUM_INSTANCES >=3) && !DT_NODE_EXISTS(BMI088_ACCEL_2)
#error "BMI088 instance 2 accelerometer node not found in device tree"
#endif
#if (CONFIG_BMI088_NUM_INSTANCES >=3) && !DT_NODE_EXISTS(BMI088_GYRO_2)
#error "BMI088 instance 2 gyroscope node not found in device tree"
#endif

#endif

// Configurazioni per il multi-instance del ICM42688P
#ifdef CONFIG_ICM42688P_DRIVER_ENABLED 

#define ICM42688P_0  DT_NODELABEL(icm42688p_0)
#define ICM42688P_1  DT_NODELABEL(icm42688p_1)
#define ICM42688P_2  DT_NODELABEL(icm42688p_2)

// Istanza 0
#if (CONFIG_ICM42688P_NUM_INSTANCES >=1) && !DT_NODE_EXISTS(ICM42688P_0)
#error "ICM42688P instance 0 accelerometer node not found in device tree"
#endif
// Istanza 1
#if (CONFIG_ICM42688P_NUM_INSTANCES >=2) && !DT_NODE_EXISTS(ICM42688P_1)
#error "ICM42688P instance 1 accelerometer node not found in device tree"
#endif
// Istanza 2
#if (CONFIG_ICM42688P_NUM_INSTANCES >=3) && !DT_NODE_EXISTS(ICM42688P_2)
#error "ICM42688P instance 2 accelerometer node not found in device tree"
#endif

#endif


#else

#define IMU_INSTANCES 1
#define GPS_INSTANCES 1

#endif