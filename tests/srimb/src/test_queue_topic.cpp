// src/test_queue_topic.cpp
#include <zephyr/ztest.h>
#include "SRIMBQueueTopic.hpp"

struct QueueMetadata {
    using DataType = uint32_t;
    static constexpr const char* TOPIC_NAME = "queue_topic";
    static constexpr uint32_t TOPIC_HASH = 0x5678;
    static constexpr uint8_t QUEUE_LENGTH = 4; // Coda corta per test di saturazione
};

using QueueTopic = srimb::SRIMBQueueTopic<QueueMetadata>;
using QueueSub = srimb::SRIMBSub<QueueTopic>;

ZTEST_SUITE(srimb_queue_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(srimb_queue_suite, test_queue_basic_pub_sub)
{
    QueueTopic topic;
    QueueSub sub(&topic);
    uint32_t out;

    topic.publish(100);
    zassert_true(sub.poll(out), "Poll should succeed");
    zassert_equal(out, 100);
}

ZTEST(srimb_queue_suite, test_queue_overwrite_oldest)
{
    QueueTopic topic;
    QueueSub sub(&topic);
    uint32_t out;

    // Riempiamo la coda (QUEUE_LENGTH = 4) e superiamola
    for (uint32_t i = 1; i <= 5; i++) {
        topic.publish(i);
    }

    // Ora la generazione più vecchia dovrebbe essere 2 (l'1 è stato sovrascritto)
    // Proviamo a leggere dal subscriber "vergine" (gen 0): dovrebbe segnalare missed
    zassert_true(sub.poll(out), "Poll should succeed");
    zassert_true(sub.get_missed(), "Missed flag should be set");
    // Il primo messaggio disponibile dovrebbe essere il 2
    zassert_equal(out, 2, "Should get the oldest available message");
}

ZTEST(srimb_queue_suite, test_queue_no_missed_when_caught_up)
{
    QueueTopic topic;
    QueueSub sub(&topic);
    uint32_t out;

    topic.publish(1);
    sub.poll(out); // ora sub è alla generazione 1

    topic.publish(2);
    topic.publish(3);
    sub.poll(out); // prende il 2
    sub.poll(out); // prende il 3

    zassert_false(sub.get_missed(), "Should not miss messages if polling fast enough");
}
