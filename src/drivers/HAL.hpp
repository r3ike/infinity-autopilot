#pragma once

#include <memory>
#include <array>

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>

#include "Vector3f.h"

#include "HAL_configs.hpp"

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
    virtual Vector3f get_raw_gyro() = 0;
    virtual Vector3f get_raw_accel() = 0;
    virtual ImuData get_imu() = 0;
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
    virtual unsigned long long write_log() = 0;
};

class HAL_Telemetry {
public:
    virtual unsigned long long micros() = 0;
};




class HAL{
public:
    HAL();
    ~HAL();

    HALState init();

    bool register_imu(std::unique_ptr<IHAL_IMU> imu_instance );       // Metodo per registare una nuova  IMU
    IHAL_IMU* get_imu_instance(uint8_t idx);                               // Metodo per prendere l'instanza di una IMU

    bool register_gps(std::unique_ptr<HAL_GPS> gps_instance);
    HAL_GPS* get_gps_instance(uint8_t idx);

    bool register_lidar(std::unique_ptr<HAL_LIDAR> lidar_instance);
    HAL_LIDAR* get_lidar_instance(uint8_t idx);

    bool register_mag(std::unique_ptr<HAL_MAG> mag_instance);
    HAL_MAG* get_mag_instance(uint8_t idx);

    bool register_baro(std::unique_ptr<HAL_BARO> baro_instance);
    HAL_BARO* get_baro_instance(uint8_t idx);

    HAL_MOTOR* get_motors_instance();
    HAL_Logging* get_sd_logging_instance();
    HAL_Telemetry* get_telemetry_instance();

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

    void _multi_instances_reset();      // Funzione che inserisce nullptr in tutte le celle degli array delle instanze dei sensori.

};


// Include delle implementazioni concrete DOPO le definizioni delle interfacce
#ifdef CONFIG_TARGET_TEENSY41

    #ifdef CONFIG_BMI088_DRIVER_ENABLED
    #include "Bmi088_driver.hpp"
    #endif

    #ifdef CONFIG_ICM42688P_DRIVER_ENABLED
    #include "ICM42688P_driver.hpp"
    #endif

    #ifdef CONFIG_BN280_DRIVER_ENABLED
    #include "Bn280_driver.hpp"
    #endif

    //#include <hal/teensy/drivers/lidar/TFLuna_driver/TFLuna_driver.hpp>
    //#include <hal/teensy/drivers/motor/Motor.hpp>
    
    //#include <hal/teensy/drivers/mag/Mag.hpp>
#elif defined(CONFIG_TARGET_SITL)
    #include <hal/SITL/HAL_sitl.hpp>
#endif