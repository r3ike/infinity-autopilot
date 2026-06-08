#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>

#include "SRIMB.hpp"
#include "HAL.hpp"

#ifdef CONFIG_LOGGER_ENABLED
#include "LoggerTask.hpp"
#endif

#include "ImuManagerTask.hpp"

#include "Scheduler.hpp"

#ifdef CONFIG_LOGGER_ENABLED
static LoggerTask loggerTask;
#endif

static ImuManagerTask imuManagerTask;

int main()
{
    return 0;
}