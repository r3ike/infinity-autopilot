#include "Scheduler.hpp"

K_THREAD_STACK_DEFINE(fast_wq_stack, 2048);
K_THREAD_STACK_DEFINE(medium_wq_stack, 2048);
K_THREAD_STACK_DEFINE(slow_wq_stack, 2048);

// Define the work queues exactly once
struct k_work_q fast_wq;
struct k_work_q medium_wq;
struct k_work_q slow_wq;

// Inizializzazione delle work queue (da chiamare in main)
void init_work_queues() {
    // Priorità: alta (fast), media (medium), bassa (slow)
    k_work_queue_start(&fast_wq, fast_wq_stack, K_THREAD_STACK_SIZEOF(fast_wq_stack), K_PRIO_PREEMPT(8), nullptr);
    k_work_queue_start(&medium_wq, medium_wq_stack, K_THREAD_STACK_SIZEOF(medium_wq_stack), K_PRIO_PREEMPT(4), nullptr);
    k_work_queue_start(&slow_wq, slow_wq_stack, K_THREAD_STACK_SIZEOF(slow_wq_stack), K_PRIO_PREEMPT(0), nullptr);
}