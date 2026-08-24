#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <cstdint>
#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "SRIMBWorkItemSub.hpp"

#include "uav_types.hpp"

/**
 * Classe che si occupa di effettuare il preprocessing dei sample delle imu:
 *  - filtering (LPF. notche, ecc..)
 *  - calibration
 *  - checks (...)
 */
class ImuPreprocessor : public WorkItemBase<Test2>, public srimb::SRIMBWorkItemSub
{

public:
    ImuPreprocessor();
    ~ImuPreprocessor() = default;

private:
    /* data */
};

