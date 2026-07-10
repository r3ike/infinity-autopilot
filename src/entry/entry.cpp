#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(uav_main, LOG_LEVEL_INF);

#include "SRIMB.hpp"
#include "HAL.hpp"

#include "uav_types.hpp"

#ifdef CONFIG_LOGGER_ENABLED
//#include "Logger.hpp"
#endif

#include "ImuManager.hpp"

#include "Test1.hpp"
#include "Test2.hpp"

#include "Scheduler.hpp"
#include "WorkQueue.hpp"

using namespace infinity_autopilot;
using namespace infinity_autopilot::tasks;
using namespace infinity_autopilot::scheduler;

K_THREAD_STACK_DEFINE(stack_test1,  Test1::taskConf.stack_size);

K_THREAD_STACK_DEFINE(stack_fast_sensors_wq,  2048);

/**----------------------------------------
 *              Work queue define
 ------------------------------------------*/

static WorkQueue fast_sensors_wq;

/**---------------------------------------------
 *              Topic instances
 ---------------------------------------------*/

static srimb::SRIMBTopic<RawAccData> raw_acc_topic[IMU_INSTANCES];
static srimb::SRIMBTopic<RawGyroData> raw_gyro_topic[IMU_INSTANCES];

static srimb::SRIMBTopic<ImuData> imus_topic[IMU_INSTANCES];

/**---------------------------------------------
 *              Task instances
 ---------------------------------------------*/

static ImuManager ImuManager<IMU_INSTANCES>(raw_acc_topic, raw_gyro_topic);
//static Test1 moduleTest1(imu_topic);
//static Test2 moduleTest2(imu_topic);

static HAL hal;
static Scheduler tasks_scheduler;

int main()
{
    //tasks_scheduler.addTask(&moduleTest1, stack_test1, Test1::taskConf.stack_size);
//
    //bool allOk = tasks_scheduler.initAllTasks();
    //if (!allOk)
    //{
    //    LOG_WRN("Alcuni task non inizializzati — procedendo comunque");
    //}

    fast_sensors_wq.start(stack_fast_sensors_wq, 2048, 3);

    hal.init(raw_acc_topic, raw_gyro_topic, fast_sensors_wq);

    
    //tasks_scheduler.start();
    
    return 0;
}