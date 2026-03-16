#pragma once 

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <sys/util.h>


// Stack per le work queue
K_THREAD_STACK_DEFINE(fast_wq_stack, 2048);
K_THREAD_STACK_DEFINE(medium_wq_stack, 2048);
K_THREAD_STACK_DEFINE(slow_wq_stack, 2048);

// Strutture delle work queue
static struct k_work_queue fast_wq;
static struct k_work_queue medium_wq;
static struct k_work_queue slow_wq;

// Inizializzazione delle work queue (da chiamare in main)
static void init_work_queues() {
    // Priorità: alta (fast), media (medium), bassa (slow)
    k_work_queue_start(&fast_wq, fast_wq_stack, K_THREAD_STACK_SIZEOF(fast_wq_stack), K_PRIO_PREEMPT(8), nullptr);
    k_work_queue_start(&medium_wq, medium_wq_stack, K_THREAD_STACK_SIZEOF(medium_wq_stack), K_PRIO_PREEMPT(4), nullptr);
    k_work_queue_start(&slow_wq, slow_wq_stack, K_THREAD_STACK_SIZEOF(slow_wq_stack), K_PRIO_PREEMPT(0), nullptr);
}