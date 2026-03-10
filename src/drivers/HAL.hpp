#pragma once

#include <memory>
#include <array>

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>

#include "Vector3f.h"


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

#define IMU_INSTANCES (BMI088_INSTANCES + 0)
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


#else

#define IMU_INSTANCES 1
#define GPS_INSTANCES 1

#endif

#include "imu_topic/ImuData.hpp"
#include "gps_topic/GpsData.hpp"
#include "lidar_topic/LidarData.hpp"



struct HALState{
    bool imu_state[IMU_INSTANCES];
    bool mag_state[MAG_INSTANCES];
    bool lidar_state[LIDAR_INSTANCES];
    bool gps_state[GPS_INSTANCES];
    bool baro_state[BARO_INSTANCES];
    bool motor_state;
    bool tlm_state;
    bool logger_state;

    //HALState():

};



/*---------------------------------
    CLASSI INTERFACE PER HAL
----------------------------------*/
class IHAL_IMU {
public:
    virtual bool init() = 0;
    virtual void calib() = 0;
    virtual Vector3f getRawGyro() = 0;
    virtual Vector3f getRawAccel() = 0;
    virtual ImuData getRawImu() = 0;
};

class HAL_GPS {
public:
#ifdef CONFIG_TARGET_TEENSY41
    virtual bool init() = 0;
#elif defined(CONFIG_TARGET_SITL)
    virtual bool init() = 0;
#endif
    virtual GpsData read() = 0;
};

class HAL_MAG {
public:
    virtual bool init() = 0;
    virtual void read() = 0;
};

class HAL_BARO {
public:
    virtual void read() = 0;
};

class HAL_LIDAR {
public:
#ifdef CONFIG_TARGET_TEENSY41
    virtual bool init() = 0;
#elif defined(CONFIG_TARGET_SITL)
    virtual bool init() = 0;
#endif
    virtual void calib() = 0;
    virtual LidarData read() = 0;
};

class HAL_MOTOR {
public:
    virtual void init() = 0;
    virtual void write(float m1, float m2, float m3, float m4) = 0;
    virtual void stop() = 0;
    virtual void test() = 0;
};

class HAL_Logging {
public:
    virtual unsigned long long writeLog() = 0;
};

class HAL_Telemetry {
public:
    virtual unsigned long long micros() = 0;
};

/**
 * HAL che gestisce il time e le interrupts
 */
class HAL_Time_Interrupts {
public:
    virtual unsigned long long micros() = 0;

    virtual void enterCritical() = 0;
    virtual void exitCritical() = 0;

    virtual void startHardLoop(uint32_t frequency, void (*callback)()) = 0;
};


class HAL{
public:
    HAL();
    ~HAL();

    HALState init();

    bool registerImu(std::unique_ptr<IHAL_IMU> imu_instance );       // Metodo per registare una nuova  IMU
    IHAL_IMU* getImuInstance(uint8_t idx);                               // Metodo per prendere l'instanza di una IMU

    bool registerGps(std::unique_ptr<HAL_GPS> gps_instance);
    HAL_GPS* getGpsInstance(uint8_t idx);

    bool registerLidar(std::unique_ptr<HAL_LIDAR> lidar_instance);
    HAL_LIDAR* getLidarInstance(uint8_t idx);

    bool registerMag(std::unique_ptr<HAL_MAG> mag_instance);
    HAL_MAG* getMagInstance(uint8_t idx);

    bool registerBaro(std::unique_ptr<HAL_BARO> baro_instance);
    HAL_BARO* getBaroInstance(uint8_t idx);

    HAL_MOTOR* getMotorsInstance();
    HAL_Logging* getSdLoggingInstance();
    HAL_Telemetry* getTelemetryInstance();
    HAL_Time_Interrupts* getTimeInstance();

private:
    std::array<std::unique_ptr<IHAL_IMU>, IMU_INSTANCES> _imu_instances;         // Buffer per le instanze delle IMU
    uint8_t _imu_count;                                             // Contatore per le imu aggiunte

    std::array<std::unique_ptr<HAL_GPS>,GPS_INSTANCES> _gps_instances;         // Buffer per le instanze delle GPS
    uint8_t _gps_count;                             // Contatore per il numero di gps aggiunti

    
    std::array<std::unique_ptr<HAL_LIDAR>, LIDAR_INSTANCES> _lidar_instances;   // Buffer per le instanze dei lidar
    uint8_t _lidar_count;                           // Contatore per il numero di lidar

    std::array<std::unique_ptr<HAL_MAG>, MAG_INSTANCES> _mag_instances;
    uint8_t _mag_count;

    std::array<std::unique_ptr<HAL_BARO>,BARO_INSTANCES> _baro_instances;
    uint8_t _baro_count;

    std::unique_ptr<HAL_MOTOR> _motor_instance;
    std::unique_ptr<HAL_Telemetry> _telemetry_instance;
    std::unique_ptr<HAL_Logging> _sd_logging_instance;
    std::unique_ptr<HAL_Time_Interrupts> _time_instance;

    void _multi_instances_reset();      // Funzione che inserisce nullptr in tutte le celle degli array delle instanze dei sensori.

};


// Include delle implementazioni concrete DOPO le definizioni delle interfacce
#ifdef CONFIG_TARGET_TEENSY41

    #ifdef CONFIG_BMI088_DRIVER_ENABLED
    #include "Bmi088_driver.hpp"
    #endif

    #ifdef CONFIG_BN280_DRIVER_ENABLED
    //#include "Bn280_driver.hpp"
    #endif

    //#include <hal/teensy/drivers/lidar/TFLuna_driver/TFLuna_driver.hpp>
    //#include <hal/teensy/drivers/motor/Motor.hpp>
    
    //#include <hal/teensy/drivers/mag/Mag.hpp>
#elif defined(CONFIG_TARGET_SITL)
    #include <hal/SITL/HAL_sitl.hpp>
#endif