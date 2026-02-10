#include "./hal/HAL.hpp"
#include "HAL.hpp"



HAL::HAL()
{
    _multi_instances_reset();
    
    #ifdef HAL_TEENSY
        registerImu(new Bmi088_driver());
        registerGps(new HAL_GPS_Teensy());

        motor = new HAL_MOTOR_Teensy();
        //telemetry = new HAL_Telemetry_Teensy();
        //logging = new HAL_Logging_Teensy();
        time = new HAL_TIME_INTERRUPTS_Teensy();
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

    for (uint8_t i = 0; i < _imu_count; i++) states.imu_state[i] = (_imu_instances[i] && _imu_instances[i]->init());            // Init tutte le imu
        

    /**
     * ATTENZIONE: La inizializzazione del gps è errata perchè sta passando a tutti la stessa seriale.
     */
    for (uint8_t i = 0; i < _gps_count; i++) states.gps_state[i] = _gps_instances[i] && _gps_instances[i]->init(&GPS_SERIAL);     // Init tutti i gps
    
    
    //states.lidar_state = lidar->init(&LIDAR_SERIAL);

#elif defined(HAL_SITL)

#endif

    return states;
}

bool HAL::registerImu(HAL_IMU *imu_instance)
{
    if (_imu_count >= IMU_INSTANCES) return false;
    
    _imu_instances[_imu_count++] = imu_instance;
    
    return true;
}

HAL_IMU* HAL::getImuInstance(uint8_t idx){
    if(idx < 0 || idx >= _imu_count) return nullptr;

    return _imu_instances[idx];
}

bool HAL::registerGps(HAL_GPS *gps_instance)
{
    if (_gps_count >= GPS_INSTANCES) return false;
    
    _gps_instances[_gps_count++] = gps_instance;
    
    return true;
}

HAL_GPS* HAL::getGpsInstance(uint8_t idx){
    if(idx < 0 || idx >= _gps_count) return nullptr;

    return _gps_instances[idx];
}



void HAL::_multi_instances_reset()
{
    for (uint8_t i = 0; i < IMU_INSTANCES; i++) _imu_instances[i] = nullptr;
    for (uint8_t i = 0; i < GPS_INSTANCES; i++) _gps_instances[i] = nullptr;
    for (uint8_t i = 0; i < LIDAR_INSTANCES; i++) _lidar_instances[i] = nullptr;
    for (uint8_t i = 0; i < BARO_INSTANCES; i++) _baro_instances[i] = nullptr;
    for (uint8_t i = 0; i < MAG_INSTANCES; i++) _mag_instances[i] = nullptr;
    
    
    _imu_count = 0;
    _gps_count = 0;
    _mag_count = 0;
    _baro_count = 0;
    _lidar_count = 0;
}
