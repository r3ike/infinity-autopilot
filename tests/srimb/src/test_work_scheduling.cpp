// src/test_work_scheduling.cpp
#include <zephyr/ztest.h>
#include "SRIMBWorkItemScheduling.hpp"
#include "SRIMBTopic.hpp"
#include "SRIMBSub.hpp"

// Mock di WorkItemBase per contare le esecuzioni
struct MockWorkItem : public WorkItemBase {
    int count{0};
    int submit()           override { count++; return 0; }
    int submitTo(WorkQueue&) override { count++; return 0; }
};

// Topic minimale per testare lo scheduling
struct SchedMetadata {
    using DataType = uint8_t;
    static constexpr const char* TOPIC_NAME = "sched_topic";
    static constexpr uint32_t TOPIC_HASH = 0xABCD;
};
using SchedTopic = srimb::SRIMBTopic<SchedMetadata>;
using SchedSub = srimb::SRIMBSub<SchedTopic>;

ZTEST_SUITE(srimb_scheduling_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(srimb_scheduling_suite, test_register_and_trigger)
{
    SchedTopic topic;
    SchedSub sub(&topic);
    MockWorkItem wi;

    zassert_true(sub.registerCallback(wi), "Registration should succeed");

    // Un publish deve triggerare il work item (required_updates di default = 1)
    topic.publish(1);
    zassert_equal(wi.count, 1, "Work item should run once");

    topic.publish(2);
    zassert_equal(wi.count, 2, "Work item should run again");
}

ZTEST(srimb_scheduling_suite, test_required_updates_batching)
{
    SchedTopic topic;
    SchedSub sub(&topic);
    MockWorkItem wi;

    sub.registerCallback(wi);
    // Impostiamo required_updates a 3
    zassert_true(topic.set_required_updates(sub, 3), "Setting required updates should succeed");

    topic.publish(1);
    zassert_equal(wi.count, 0, "Should not trigger yet (1/3)");

    topic.publish(2);
    zassert_equal(wi.count, 0, "Should not trigger yet (2/3)");

    topic.publish(3);
    zassert_equal(wi.count, 1, "Should trigger on 3rd publish");

    // Il contatore degli update deve essersi resettato
    topic.publish(4);
    zassert_equal(wi.count, 1, "Should not trigger immediately after reset");
}
