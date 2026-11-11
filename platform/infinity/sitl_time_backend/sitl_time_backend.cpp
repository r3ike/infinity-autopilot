#include "sitl_time_backend.h"
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/drivers/timer/system_timer.h>

static uint64_t current_sim_time_us = 0;
static uint32_t accumulated_ticks = 0;

void sitl_time_update(uint64_t new_time_us) {
    if (current_sim_time_us == 0) {
        current_sim_time_us = new_time_us;
    }

    if (new_time_us > current_sim_time_us) {
        uint32_t dt_us = (uint32_t)(new_time_us - current_sim_time_us);
        uint32_t ticks_to_add = k_us_to_ticks_floor32(dt_us);
        
        current_sim_time_us = new_time_us;
        accumulated_ticks += ticks_to_add;

        // Avanza il tempo nel kernel Zephyr
        sys_clock_announce(ticks_to_add);
    }
}

// --- API Standard di Zephyr per il Timer ---
extern "C" {
    void sys_clock_set_timeout(int32_t ticks, bool idle) {}
    uint32_t sys_clock_elapsed(void) { return 0; }
    uint32_t sys_clock_cycle_get_32(void) { return accumulated_ticks; }
}

int sitl_timer_init(void) {
    // Nessun thread creato qui! Il backend è puramente passivo.
    return 0;
}

// Registra il driver
SYS_INIT(sitl_timer_init, PRE_KERNEL_2, CONFIG_SYSTEM_CLOCK_INIT_PRIORITY);
