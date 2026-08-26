#include <zephyr/ztest.h>
#include "SRIMB.hpp"
#include "test_fakes.hpp"

struct srimb_topic_fixture {
    srimb::SRIMBTopic<int> topic;
    // I fake creati dinamicamente se necessario
    // La fixture stessa li contiene, ma possiamo crearli nei test.
};

static void *srimb_topic_setup(void) {
    srimb_topic_fixture *f = new srimb_topic_fixture;
    return f;
}

static void srimb_topic_teardown(void *f) {
    delete static_cast<srimb_topic_fixture*>(f);
}

ZTEST_SUITE(srimb_topic_tests, NULL, srimb_topic_setup, NULL, NULL, srimb_topic_teardown);

ZTEST_F(srimb_topic_tests, test_publish_updates_generation_and_data)
{
    auto *f = static_cast<srimb_topic_fixture*>(this->fixture);
    int data = 42;

    f->topic.publish(data);

    // Non esponiamo membri privati, quindi verifichiamo via poll.
    FakeSub sub;
    int out = 0;
    bool polled = f->topic.poll(sub, out);

    zassert_true(polled, "Dovrebbe ritornare true al primo poll");
    zassert_equal(out, 42, "Dovrebbe aver pubblicato 42");
    zassert_equal(sub.get_last_generation(), 1, "Generazione incrementata");
}