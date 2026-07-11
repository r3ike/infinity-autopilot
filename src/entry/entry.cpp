#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(uav_main, LOG_LEVEL_INF);

#include "SRIMB.hpp"
#include "HAL.hpp"
#include "HAL_configs.hpp"

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

K_THREAD_STACK_DEFINE(stack_fast_sensors_wq,  4096);

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

static ImuManager<IMU_INSTANCES> imu_manager(raw_acc_topic, raw_gyro_topic);
//static Test1 moduleTest1(imu_topic);
//static Test2 moduleTest2(imu_topic);

static HAL hal;
static Scheduler tasks_scheduler;

int main()
{   
    k_sleep(K_MSEC(100));
    LOG_INF("----------------------------------------------------------");
    LOG_INF("------------------- Infinity-Autopilot -------------------");
    LOG_INF("----------------------------------------------------------");
    LOG_INF("Author: Riccardo Pogliacomi");
    LOG_INF("Version: 0.5");
    //tasks_scheduler.addTask(&moduleTest1, stack_test1, Test1::taskConf.stack_size);
//
    //bool allOk = tasks_scheduler.initAllTasks();
    //if (!allOk)
    //{
    //    LOG_WRN("Alcuni task non inizializzati — procedendo comunque");
    //}

    LOG_INF("imu count: %d", IMU_INSTANCES);

    fast_sensors_wq.start(stack_fast_sensors_wq, 4096, 3);

    hal.init(raw_acc_topic, raw_gyro_topic, fast_sensors_wq);

    
    //tasks_scheduler.start();
    
    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}