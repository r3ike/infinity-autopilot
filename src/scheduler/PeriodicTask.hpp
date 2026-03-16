#pragma once

#include <zephyr/kernel.h>

class PeriodicTask {
public:
    // Constructor with function pointer and context
    PeriodicTask(void (*func)(void*), void* context,
                 struct k_work_q* queue, k_timeout_t period)
        : func_(func), context_(context), queue_(queue), period_(period) {
        k_work_init_delayable(&work_, work_handler);
    }

    void start() {
        // Use the queue-specific schedule function
        k_work_schedule_for_queue(queue_, &work_, period_);
    }

    void stop() {
        k_work_cancel_delayable(&work_);
    }

private:
    void (*func_)(void*);
    void* context_;
    struct k_work_q* queue_;
    k_timeout_t period_;
    struct k_work_delayable work_;

    static void work_handler(struct k_work* work) {
        auto* self = CONTAINER_OF(work, PeriodicTask, work_);
        self->func_(self->context_);
        // Reschedule with the same queue
        k_work_reschedule_for_queue(self->queue_, &self->work_, self->period_);
    }
};