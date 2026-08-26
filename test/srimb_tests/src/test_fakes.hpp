#ifndef TEST_FAKES_HPP
#define TEST_FAKES_HPP

#include <unordered_map>
#include <zephyr/kernel.h>
#include "SRIMBSub.hpp"   // per srimb::SRIMBWorkItemSub

class FakeSRIMBWorkItemSub : public srimb::SRIMBWorkItemSub
{
public:
    bool executed = false;
    int call_count = 0;
    struct k_work work;
    struct k_work_q *queue = nullptr;   // se nullptr usa system wq

    FakeSRIMBWorkItemSub();
    ~FakeSRIMBWorkItemSub();

    static void work_handler(struct k_work *item);

    struct k_work* getWorkItem() override { return &work; }
    struct k_work_q* getWorkQueue() override { return queue; }
};

// Mappa globale: dichiarata dopo la classe
static std::unordered_map<struct k_work*, FakeSRIMBWorkItemSub*> work_to_instance;

// Implementazioni (inline) che usano la mappa
inline FakeSRIMBWorkItemSub::FakeSRIMBWorkItemSub() {
    k_work_init(&work, &FakeSRIMBWorkItemSub::work_handler);
    work_to_instance[&work] = this;
}

inline FakeSRIMBWorkItemSub::~FakeSRIMBWorkItemSub() {
    work_to_instance.erase(&work);
}

inline void FakeSRIMBWorkItemSub::work_handler(struct k_work *item) {
    auto it = work_to_instance.find(item);
    if (it != work_to_instance.end()) {
        FakeSRIMBWorkItemSub *self = it->second;
        self->executed = true;
        self->call_count++;
    }
}

#endif