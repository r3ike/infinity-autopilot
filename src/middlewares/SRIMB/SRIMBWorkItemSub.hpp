#pragma once
#include <zephyr/kernel.h>
/**
 * Classe che bisogna ereditare se un modulo vuole sottoscrivere un workitem ad una workqueue
 * e usare come trigger la pubblicazione su uno specifico topic.
 */

class SRIMBWorkItemSub {
public:
    // Restituisce il work item da sottomettere quando arriva un evento
    virtual struct k_work* getWorkItem() = 0;
    // Eventuale coda su cui eseguire il work (può essere nullptr per system wq)
    virtual struct k_work_q* getWorkQueue() = 0;
    virtual ~SRIMBWorkItemSub() = default;
};

 