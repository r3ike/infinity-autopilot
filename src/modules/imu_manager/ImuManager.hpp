#pragma once

#include <cstddef>
#include <cstdint>
#include "imu_preprocessor/ImuPreprocessor.hpp"
#include "SRIMB.hpp"

#include "HAL_configs.hpp"

#include <zephyr/kernel.h>

/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 */

template <size_t NUM_IMU_INSTANCES>        // NUM_IMU_INSTANCES è il numero totale di instanze imu
class ImuManager
{
public:
    ImuManager(srimb::SRIMBTopicManager& topicManager)
    {
        for(int inst_id = 0; inst_id < NUM_IMU_INSTANCES; inst_id++){
            preprocessors_[count_++] = std::make_unique<ImuPreprocessor>(inst_id, topicManager);
        }
    };

    ~ImuManager() = default;

    void init() {
    };

private:

    std::array<std::unique_ptr<ImuPreprocessor>, NUM_IMU_INSTANCES> preprocessors_;
    size_t count_ {0};
};


