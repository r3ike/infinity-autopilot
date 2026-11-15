// src/test_manager.cpp
#include "SRIMB.hpp"
#include <zephyr/ztest.h>
#include "SRIMBTopic.hpp"

struct MgrMetadata {
    using DataType = uint16_t;
    static constexpr const char* TOPIC_NAME = "mgr_topic";
    static constexpr uint32_t TOPIC_HASH = 0xDEAD;
};
using MgrTopic = srimb::SRIMBTopic<MgrMetadata>;

ZTEST_SUITE(srimb_manager_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(srimb_manager_suite, test_create_and_reuse)
{
    srimb::SRIMBTopicManager mgr;

    // Prima creazione per instance_id 1
    srimb::SRIMBTopicBase* base1 = mgr.getOrCreateTopic<MgrTopic>(1);
    zassert_not_null(base1);

    // Seconda chiamata con lo stesso ID deve restituire lo stesso puntatore
    srimb::SRIMBTopicBase* base1_again = mgr.getOrCreateTopic<MgrTopic>(1);
    zassert_equal_ptr(base1, base1_again, "Should return the same instance for same ID");
}

ZTEST(srimb_manager_suite, test_different_instances)
{
    srimb::SRIMBTopicManager mgr;

    srimb::SRIMBTopicBase* base1 = mgr.getOrCreateTopic<MgrTopic>(1);
    srimb::SRIMBTopicBase* base2 = mgr.getOrCreateTopic<MgrTopic>(2);

    zassert_not_equal(base1, base2, "Different IDs should yield different instances");
}
