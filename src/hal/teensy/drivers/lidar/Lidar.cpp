#include <drivers/lidar/Lidar.hpp>
#include "Lidar.hpp"

// Costructor
Lidar::Lidar(){_alt_calib_offset=0;}
Lidar::~Lidar(){}

bool Lidar::init(Stream *serialPtr){
    bool res = _lidar.begin(serialPtr);
    if (res)
    {
        _lidar.sendCommand(SET_FRAME_RATE, FRAME_20);
    }
    
    return res;
}

/*
int16_t Lidar::read()
{
    int16_t lidar_altitude = 0.0f;

    lidar.getData(lidar_altitude);

    return lidar_altitude;
}
*/

void Lidar::calib(){
    _alt_calib_offset = 0;
    for (int i = 0; i < LIDAR_CALIBRATION_STEP; i++)
    {
        _alt_calib_offset += read().altitude;
    }
    
    _alt_calib_offset = _alt_calib_offset/LIDAR_CALIBRATION_STEP;
}

LidarData Lidar::read(){
    int16_t lidar_altitude = 0.0f;
    int16_t tfFlux = 0;    // Strength or quality of return signal
    int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip

    _lidar.getData(lidar_altitude, tfFlux, tfTemp);

    return {lidar_altitude - _alt_calib_offset, tfFlux, tfTemp};
}
