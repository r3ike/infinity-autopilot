#include <zephyr/ztest.h>
#include "SRIMBQueueTopic.hpp"
#include "SRIMBSub.hpp"
#include "test_fakes.hpp"

ZTEST_SUITE(srimb_queue_topic_tests, NULL, NULL, NULL, NULL, NULL);

ZTEST_F(srimb_queue_topic_tests, test_publish_poll_queue)
{
    srimb::SRIMBQueueTopic<int, 8> topic;

    int first_data = 42;
    topic.publish(first_data);

    int second_data = 50;
    topic.publish(second_data);

    srimb::SRIMBSub sub;

    int first_poll = 0;
    bool first_polled = topic.poll(sub, first_poll);

    int second_poll = 0;
    bool second_polled = topic.poll(sub, second_poll);

    int third_poll = 0;
    bool third_polled = topic.poll(sub, third_poll);

    zassert_equal(first_poll, first_data, "primo poll fallito");
    zassert_equal(second_poll, second_data, "secondo poll fallito");

    zassert_equal(third_poll, 0, "terzo poll sbagliato");

    zassert_true(first_polled, "flag primo poll false => dovrebbe essere true");
    zassert_true(second_polled, "flag secondo poll false => dovrebbe essere true");

    zassert_false(third_polled, "flag secondo poll true => dovrebbe essere false");

}