#include "./hal/HAL.hpp"
#include "HAL.hpp"



HAL::HAL()
{
    _multi_instances_reset();
    
    #ifdef HAL_TEENSY
        registerImu(std::make_unique<Bmi088_driver>());
        registerGps(std::make_unique<Bn280_driver>());
        //registerBaro(std::make_unique);
        registerLidar(std::make_unique<HAL_LIDAR_Teensy>());
        registerMag(std::make_unique<HAL_MAG_Teensy>());

        _motor_instance = std::make_unique<HAL_MOTOR_Teensy>();
        _time_instance = std::make_unique<HAL_TIME_INTERRUPTS_Teensy>();
        //_telemetry_instance
        //_sd_logging_instance

    #elif defined(HAL_SITL)
        registerImu(new HAL_IMU_SITL());
        //pwm = new HAL_MOTOR_SITL();
        //telemetry = new HAL_Telemetry_SITL();
        //logging = new HAL_Logging_SITL();
        time = new HAL_TIME_INTERRUPTS_SITL();
    #endif
}
HAL::~HAL(){}

HALState HAL::init()
{
    HALState states;
#ifdef HAL_TEENSY

    GPS_SERIAL.begin(9600);
    LIDAR_SERIAL.begin(9600);

    for (uint8_t i = 0; i < _imu_count; i++) states.imu_state[i] = (_imu_instances.at(i)->init());            // Init tutte le imu
        

    /**
     * ATTENZIONE: La inizializzazione del gps è errata perchè sta passando a tutti la stessa seriale.
     */
    for (uint8_t i = 0; i < _gps_count; i++) states.gps_state[i] = _gps_instances.at(i)->init(&GPS_SERIAL);     // Init tutti i gps
    
    
    //states.lidar_state = lidar->init(&LIDAR_SERIAL);

#elif defined(HAL_SITL)

#endif

    return states;
}



bool HAL::registerImu(std::unique_ptr<HAL_IMU> imu_instance )
{
    if (_imu_count >= IMU_INSTANCES) return false;
    
    _imu_instances[_imu_count++] = std::move(imu_instance);
    
    return true;
}

HAL_IMU* HAL::getImuInstance(uint8_t idx){
    if(idx >= _imu_count) return nullptr;

    return _imu_instances.at(idx).get();
}

bool HAL::registerGps(std::unique_ptr<HAL_GPS> gps_instance)
{
    if (_gps_count >= GPS_INSTANCES) return false;
    
    _gps_instances[_gps_count++] = std::move(gps_instance);
    
    return true;
}

HAL_GPS* HAL::getGpsInstance(uint8_t idx){
    if(idx >= _gps_count) return nullptr;

    return _gps_instances.at(idx).get();
}

bool HAL::registerLidar(std::unique_ptr<HAL_LIDAR> lidar_instance)
{
    if (_lidar_count >= LIDAR_INSTANCES) return false;
    
    _lidar_instances[_lidar_count++] = std::move(lidar_instance);
    
    return true;
}

HAL_LIDAR *HAL::getLidarInstance(uint8_t idx)
{
    if(idx >= _lidar_count) return nullptr;

    return _lidar_instances.at(idx).get();
}

bool HAL::registerMag(std::unique_ptr<HAL_MAG> mag_instance)
{
    if (_mag_count >= MAG_INSTANCES) return false;
    
    _mag_instances[_mag_count++] = std::move(mag_instance);
    
    return true;
}

HAL_MAG *HAL::getMagInstance(uint8_t idx)
{
    if(idx >= _mag_count) return nullptr;

    return _mag_instances.at(idx).get();
}

bool HAL::registerBaro(std::unique_ptr<HAL_BARO> baro_instance)
{
    if (_mag_count >= BARO_INSTANCES) return false;
    
    _baro_instances[_baro_count++] = std::move(baro_instance);
    
    return true;
}

HAL_BARO *HAL::getBaroInstance(uint8_t idx)
{
    if(idx >= _baro_count) return nullptr;

    return _baro_instances.at(idx).get();
}

HAL_MOTOR *HAL::getMotorsInstance()
{
    return _motor_instance.get();
}

HAL_Logging *HAL::getSdLoggingInstance()
{
    return _sd_logging_instance.get();
}

HAL_Telemetry *HAL::getTelemetryInstance()
{
    return _telemetry_instance.get();
}

HAL_Time_Interrupts *HAL::getTimeInstance()
{
    return _time_instance.get();
}



void HAL::_multi_instances_reset()
{
    /*
    _imu_instances.fill(nullptr);
    _gps_instances.fill(nullptr);
    _lidar_instances.fill(nullptr);
    _baro_instances.fill(nullptr);
    _mag_instances.fill(nullptr);
    */
    
    _imu_count = 0;
    _gps_count = 0;
    _mag_count = 0;
    _baro_count = 0;
    _lidar_count = 0;
    /*
    _motor_instance = nullptr;
    _telemetry_instance = nullptr;
    _sd_logging_instance = nullptr;
    _time_instance = nullptr;
    */
}
