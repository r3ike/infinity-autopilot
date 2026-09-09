#pragma once
#include <zephyr/kernel.h>
#include "WorkQueue.hpp"

#include "WorkItemBase.hpp"

template<typename Derived> : public WorkItemBase
class WorkItem {
public:
    WorkItemBase() {
        k_work_init(&work_, &WorkItem::trampoline);
    }

    int submit() override { return k_work_submit(&work_); }
    int submitTo(WorkQueue &wq) override {
        return k_work_submit_to_queue(wq.c_ptr(), &work_);
    }
    // ... altri metodi ...

private:
    static void trampoline(struct k_work *work) {
        // Recupera il puntatore all'oggetto
        Derived *obj = CONTAINER_OF(work, Derived, work_);
        obj->handler();
    }

    struct k_work work_;
    friend Derived; // permette a Derived di accedere a work_
};