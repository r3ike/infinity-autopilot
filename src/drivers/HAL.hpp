#pragma once

#include <memory>
#include <array>

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <cstdint>  

#include "WorkQueue.hpp"
#include "Vector3f.h"
#include "SRIMB.hpp"
#include "HAL_configs.hpp"

#include "uav_types.hpp"



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
private: 
    static constexpr size_t MAX_STR_LEN = 32;

    uint8_t id_ {0};
    const char* model_;    // modello sensore (es: bmi088)
    char name_[MAX_STR_LEN];     // nome sensore (es: bmi088_1)
public:
    IHAL_IMU(const char* model) : id_(0xFF), model_(model) {
        
        //snprintk(name_, MAX_STR_LEN, "%s_%u", model_, id_);
    };

    void set_id(uint8_t id){id_ = id;}
    
    virtual bool init(uint8_t unique_id, srimb::SRIMBTopic<RawAccData>& acc_topic, srimb::SRIMBTopic<RawGyroData>& gyro_topic, WorkQueue& wq) = 0;
};

class HAL_GPS {
public:
    virtual bool init() = 0;
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
    virtual bool init() = 0;
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

class IHAL_Storage {
public:
    virtual uint64_t write() = 0;
};

class IHAL_Radio {
public:
    virtual bool init() = 0;

    virtual void send_to_GCS(uint8_t* buffer, size_t len) = 0;         // Metodo per inviare alla Ground control station

    virtual long recive_from_GCS(uint8_t* buffer, size_t len) = 0;     // Metodo per ricevere dall Ground control station
};


class HAL{
public:
    HAL();
    ~HAL();

    void init(
        srimb::SRIMBTopic<RawAccData> (&raw_acc_topic)[IMU_INSTANCES],
        srimb::SRIMBTopic<RawGyroData> (&raw_gyro_topic)[IMU_INSTANCES],
        WorkQueue& fast_sensors_wq
    );

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
    IHAL_Storage* get_sd_instance();
    IHAL_Radio* get_telemetry_instance();

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
    std::unique_ptr<IHAL_Radio> _radio_instance;
    std::unique_ptr<IHAL_Storage> _sd_instance;

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
    #ifdef CONFIG_RADIO_SITL_DRIVER_ENABLED
    #include "Radio_sitl.hpp"
    #endif
#endif