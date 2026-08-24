// Auto-generated from RawGyro.topic - DO NOT EDIT
//                   Infinity-Autopilot
#pragma once
#include <cstdint>
#include <cstddef>

struct RawGyroData {

    uint64_t timestamp;
    uint8_t id;
    float x;
    float y;
    float z;
    float temperature;
};