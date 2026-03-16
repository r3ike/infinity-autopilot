// periodic_task.h
#pragma once

#include <zephyr/kernel.h>
#include <functional>

class PeriodicTask {
public:
    // Il costruttore accetta un callable (es. std::function<void()>) e la work queue
    PeriodicTask(std::function<void()> func, struct k_work_queue& queue, k_timeout_t period) : func_(std::move(func)), queue_(queue), period_(period) {
        k_work_init_delayable(&work_, work_handler);
    }

    // Avvia la schedulazione periodica
    void start() {
        k_work_schedule(&queue_, &work_, period_);
    }

    // Ferma il task (opzionale)
    void stop() {
        k_work_cancel_delayable(&work_);
    }

private:
    std::function<void()> func_;
    struct k_work_queue& queue_;
    k_timeout_t period_;
    struct k_work_delayable work_;

    static void work_handler(struct k_work* work) {
        auto* self = CONTAINER_OF(work, PeriodicTask, work_);
        self->func_();  // Esegue la funzione associata

        // Ripianifica
        k_work_reschedule(&self->queue_, &self->work_, self->period_);
    }
};