#pragma once 

#include <utils/Vector3f.h>
#include <./hal/teensy/lib/TFMPlus/TFMPlus.h>
#include <config/parameters.h>
#include <utils/uav_data.hpp>

class Lidar
{
private:
    TFMPlus _lidar;

    int16_t _alt_calib_offset;

public:
    Lidar();

    bool init(Stream *serialPtr);

    //int16_t read();
    void calib();

    LidarData read();

    ~Lidar();
};


