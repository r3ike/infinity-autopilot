#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(uav_main, LOG_LEVEL_INF);

#include "SRIMB.hpp"
#include "HAL.hpp"

#include "uav_types.hpp"

#ifdef CONFIG_LOGGER_ENABLED
//#include "LoggerTask.hpp"
#endif

//#include "ImuManagerTask.hpp"

#include "Test1.hpp"
#include "Test2.hpp"

#include "Scheduler.hpp"

using namespace infinity;
using namespace infinity::tasks;
using namespace infinity::scheduler;

K_THREAD_STACK_DEFINE(stack_test1,  Test1::taskConf.stack_size);

/**---------------------------------------------
 *              Topic instances
 ---------------------------------------------*/

static srimb::SRIMBTopic<ImuData> imu_topic;

/**---------------------------------------------
 *              Task instances
 ---------------------------------------------*/
#ifdef CONFIG_LOGGER_ENABLED
//static LoggerTask loggerTask;
#endif

//static ImuManagerTask imuManagerTask;
static Test1 moduleTest1(imu_topic);
static Test2 moduleTest2(imu_topic);

static Scheduler tasks_scheduler;

int main()
{
    tasks_scheduler.addTask(&moduleTest1, stack_test1, Test1::taskConf.stack_size);

    bool allOk = tasks_scheduler.initAllTasks();
    if (!allOk)
    {
        LOG_WRN("Alcuni task non inizializzati — procedendo comunque");
    }

    tasks_scheduler.start();
    
    return 0;
}