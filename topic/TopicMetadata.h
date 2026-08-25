// Auto-generated from topic folder - DO NOT EDIT
//                   Infinity-Autopilot
#pragma once
#include <cstdint>
#include <cstddef>
namespace srimb{


struct RawGyroMetadata {
    static constexpr const char* TOPIC_NAME = "RawGyro";
    static constexpr size_t TOPIC_SIZE = sizeof(RawGyro);
    static constexpr uint32_t TOPIC_HASH = 0xb01f5668;
    static constexpr uint8_t QUEUE_LENGTH = 8;
};

struct RawAccMetadata {
    static constexpr const char* TOPIC_NAME = "RawAcc";
    static constexpr size_t TOPIC_SIZE = sizeof(RawAcc);
    static constexpr uint32_t TOPIC_HASH = 0x69fb8237;
};

}