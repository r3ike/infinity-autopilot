#pragma once

#include <zephyr/kernel.h>
#include "Modules.hpp"

class PeriodicWork {
public:
    // Il work item chiamerà il metodo run() del modulo associato
    PeriodicWork(Module& module, struct k_work_queue& queue, k_timeout_t period) : module_(module), queue_(queue), period_(period) {
        k_work_init_delayable(&work_, work_handler);
    }

    // Avvia la schedulazione periodica
    void start() {
        // La prima esecuzione avviene dopo un periodo
        k_work_schedule(&queue_, &work_, period_);
    }

    // Ferma il lavoro (opzionale)
    void stop() {
        k_work_cancel_delayable(&work_);
    }

private:
    Module& module_;
    struct k_work_queue& queue_;
    k_timeout_t period_;
    struct k_work_delayable work_;

    // Handler statico che richiama il modulo
    static void work_handler(struct k_work* work) {
        auto* self = CONTAINER_OF(work, PeriodicWork, work_);
        self->module_.run();

        // Ripianifica per il prossimo periodo
        k_work_reschedule(&self->queue_, &self->work_, self->period_);
    }
};