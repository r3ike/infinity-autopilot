#pragma once

#include <cstdint>
#include <zephyr/kernel.h>
#include "WorkQueue.hpp"

template<typename Derived>
class WorkItemDelayable
{

public:
    WorkItemDelayable(uint64_t period_us) {
        k_work_init_delayable(&work_delayable_, WorkItemDelayable::trampoline);
        
        setPeriodUs(period_us);
    }
    ~WorkItemDelayable() { cancelWork();};

    void setPeriodUs(uint64_t period){period_us_ = period;}

    int submit() {
        return k_work_schedule(&work_delayable_, K_USEC(period_us_));
    }
    int submitTo(WorkQueue& wq){
        return k_work_schedule_for_queue(wq.c_ptr(), &work_delayable_, K_USEC(period_us_));
    }

    int cancelWork() {return k_work_cancel_delayable(&work_delayable_);}

private:
    static void trampoline(struct k_work *work) {
        // Recupera il puntatore all'oggetto
        struct k_work_delayable *dwork = k_work_delayable_from_work(work);
        Derived *obj = CONTAINER_OF(dwork, Derived, work_delayable_);
        obj->handler();

        k_work_reschedule(&work_delayable_, K_USEC(obj->period_us_));
    }

    uint64_t period_us_ {0};
    struct k_work_delayable work_delayable_;
    friend Derived;
};

