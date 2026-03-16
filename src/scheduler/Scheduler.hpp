#pragma once 

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/util.h>



// Strutture delle work queue
extern struct k_work_q fast_wq;
extern struct k_work_q medium_wq;
extern struct k_work_q slow_wq;

void init_work_queues();