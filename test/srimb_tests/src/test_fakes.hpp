#pragma once 
#include "SRIMBSub.hpp"
#include "SRIMBWorkItemSub.hpp"
#include <zephyr/kernel.h>

class FakeSRIMBSub : public srimb::SRIMBSub
{
private:
    uint64_t last_gen = 0;
public:
    uint64_t get_last_generation() const override { return last_gen; }
    void set_last_generation(uint64_t gen) override { last_gen = gen; }
};

class FakeSRIMBWorkItemSub : public srimb::SRIMBWorkItemSub
{
public:
    bool executed = false;
    int call_count = 0;
    struct k_work work;
    struct k_work_q *queue = nullptr;  // se nullptr usa system wq

    FakeWorkItemSub() {
        k_work_init(&work, work_handler);
        work.arg = this;  // così l'handler può recuperare l'oggetto
    }

    static void work_handler(struct k_work *item) {
        FakeWorkItemSub *self = static_cast<FakeWorkItemSub*>(item->arg);
        self->executed = true;
        self->call_count++;
    }

    // Override dei metodi virtuali
    struct k_work* getWorkItem() override { return &work; }
    struct k_work_q* getWorkQueue() override { return queue; }
};

