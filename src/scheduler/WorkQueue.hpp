
#pragma once
#include <zephyr/kernel.h>
#include <utility>

class WorkQueue {
public:
    // stack_size in bytes, priority: thread priority
    WorkQueue(size_t stack_size, int priority) : started_(false){
        k_work_queue_init(&queue_);
    }

    // Avvia il thread della work queue
    void start(k_thread_stack_t *stack, size_t stack_size, int priority) {
        k_work_queue_start(&queue_, stack, stack_size, priority, nullptr);
        started_ = true;

        /**
         * TODO:
         *  - capire se si può impostare il thread delle workqueue con EDF e se è necessario
         */
    }

    // Ottenere la k_work_q sottostante
    struct k_work_q* c_ptr() { return &queue_; }

    // Non copiabile, non mobile (perché il thread è in esecuzione)
    WorkQueue(const WorkQueue&) = delete;
    WorkQueue& operator=(const WorkQueue&) = delete;

private:
    struct k_work_q queue_;
    bool started_;
};