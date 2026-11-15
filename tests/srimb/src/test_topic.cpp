// src/test_topic.cpp
#include <zephyr/ztest.h>
#include "SRIMBTopic.hpp"
#include "SRIMBSub.hpp"
#include "SRIMBPub.hpp"

struct TestMetadata {
    using DataType = uint32_t;
    static constexpr const char* TOPIC_NAME = "test_topic";
    static constexpr uint32_t TOPIC_HASH = 0x1234;
};

using TestTopic = srimb::SRIMBTopic<TestMetadata>;
using TestSub = srimb::SRIMBSub<TestTopic>;
using TestPub = srimb::SRIMBPub<TestTopic>;

ZTEST_SUITE(srimb_topic_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(srimb_topic_suite, test_pub_sub_basic)
{
    TestTopic topic;
    TestSub sub(&topic);
    TestPub pub(&topic);
    uint32_t out = 0;

    // Poll iniziale: nessun dato
    zassert_false(sub.poll(out), "Poll should fail before any publish");

    // Pubblica e verifica
    zassert_true(pub.publish(42), "Publish should succeed");
    zassert_true(sub.updated(), "Subscriber should be updated");
    zassert_true(sub.poll(out), "Poll should succeed after publish");
    zassert_equal(out, 42, "Data mismatch");
}

ZTEST(srimb_topic_suite, test_generation_advances)
{
    TestTopic topic;
    TestSub sub(&topic);
    TestPub pub(&topic);
    uint32_t out;

    pub.publish(1);
    sub.poll(out);
    zassert_equal(sub.get_last_generation(), 1, "Generation should be 1");

    pub.publish(2);
    sub.poll(out);
    zassert_equal(sub.get_last_generation(), 2, "Generation should be 2");
}

ZTEST(srimb_topic_suite, test_subscriber_isolation)
{
    TestTopic topic;
    TestSub sub1(&topic);
    TestSub sub2(&topic);
    TestPub pub(&topic);
    uint32_t out1, out2;

    pub.publish(10);
    zassert_true(sub1.poll(out1), "Sub1 poll should succeed");
    zassert_true(sub2.poll(out2), "Sub2 poll should succeed");
    zassert_equal(out1, 10);
    zassert_equal(out2, 10);
}
