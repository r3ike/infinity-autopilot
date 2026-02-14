#pragma once 
#include <hal/HAL.hpp>
#include <utils/Vector3f.h>
#include <hal/teensy/lib/TFMPlus/TFMPlus.h>
#include <config/parameters.h>
#include <utils/srimb_topics/lidar_topic/lidar_topic.hpp>

class TFLuna_driver : public HAL_LIDAR
{
private:
    TFMPlus _lidar;

    int16_t _alt_calib_offset;

public:
    TFLuna_driver();
    ~TFLuna_driver() = default;

    bool init(Stream *serialPtr) override;

    //int16_t read();
    void calib() override;

    LidarData read() override;

};


