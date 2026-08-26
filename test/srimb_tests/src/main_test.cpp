#include <zephyr/ztest.h>
#include "SRIMB.hpp"
#include "SRIMBSub.hpp"
#include "test_fakes.hpp"

//struct srimb_topic_tests_fixture {
//    srimb::SRIMBTopic<int> topic;
//    // I fake creati dinamicamente se necessario
//    // La fixture stessa li contiene, ma possiamo crearli nei test.
//};
//
//static void *srimb_topic_setup(void) {
//    srimb_topic_tests_fixture *f = new srimb_topic_tests_fixture;
//    return f;
//}
//
//static void srimb_topic_teardown(void *f) {
//    delete static_cast<srimb_topic_tests_fixture*>(f);
//}

ZTEST_SUITE(srimb_topic_tests, NULL, NULL, NULL, NULL, NULL);

/**
 * Test case 1:
 *  - controlla le funzionalità base del publish e del poll
 *  - controlla se il poll restituisce correttamente il flag 
 *  - controlla se il valore in uscita dal poll è uguale al valore pubblicato
 *  - controlla se la generazione è stata correttamente incrementata dal sub
 */
ZTEST_F(srimb_topic_tests, test_publish_updates_generation_and_data)
{
    srimb::SRIMBTopic<int> topic;
    int data = 42;

    topic.publish(data);

    // Non esponiamo membri privati, quindi verifichiamo via poll.
    srimb::SRIMBSub sub;
    int out = 0;
    bool polled = topic.poll(sub, out);

    printk("polled=%d, out=%d, gen=%d\n", polled, out, sub.get_last_generation());

    zassert_true(polled, "Dovrebbe ritornare true al primo poll");
    zassert_equal(out, 42, "Dovrebbe aver pubblicato 42");
    zassert_equal(sub.get_last_generation(), 1, "Generazione incrementata");
}

/**
 * Test case 2:
 *  - controllo di più poll consecutivi senza nuovi publish
 */

 ZTEST_F(srimb_topic_tests, test_poll_return_false_when_no_updates)
{
    srimb::SRIMBTopic<int> topic;

    int data = 42;

    topic.publish(data);

    srimb::SRIMBSub sub;
    int out = 0;
    bool polled = topic.poll(sub, out);

    int out_second = 0;
    bool polled_second = topic.poll(sub, out_second);

    zassert_equal(out_second, 0, "Dovrebbe essere 0");
    zassert_true(polled, "dovrebbe essere true al primo poll");
    zassert_false(polled_second, "dovrebbe essere false al secondo poll");
}

/**
 * Test case 3:
 *  - controllo metodo updated()
 */

ZTEST_F(srimb_topic_tests, test_updated_method)
{
    srimb::SRIMBTopic<int> topic;

    int data = 42;

    topic.publish(data);

    srimb::SRIMBSub sub;
    bool first_updated = topic.updated(sub);

    int out = 0;
    topic.poll(sub, out);

    bool second_updated = topic.updated(sub);

    topic.publish(data);

    bool third_updated = topic.updated(sub);

    zassert_true(first_updated, "dovrebbe essere true se il sub non ha ancora fatto il poll del nuovo messaggio");
    zassert_false(second_updated, "dovrebbe essere false dopo che il sub ha fatto il poll");
    zassert_true(third_updated, "Dovrebbe tornare true dopo un'altro publish");
}

/**
 * Test case 3:
 *  - test per verificare se alla pubblicazione di un msg viene fatto il sub del work item registrato
 */
ZTEST_F(srimb_topic_tests, test_work_item_submit)
{
    srimb::SRIMBTopic<int> topic;

    static K_THREAD_STACK_DEFINE(stack, 1024);

    struct k_work_q wq;

    k_work_queue_init(&wq);

    k_work_queue_start(&wq, stack, K_THREAD_STACK_SIZEOF(stack), K_PRIO_COOP(1), NULL);

    FakeSRIMBWorkItemSub fake_work_item1;
    fake_work_item1.queue = &wq; 

    FakeSRIMBWorkItemSub fake_work_item2;
    fake_work_item2.queue = nullptr;

    zassert_true(topic.register_work_item(&fake_work_item1), "Registrazione work item 1 fallita");
    zassert_true(topic.register_work_item(&fake_work_item2), "Registrazione work item 2 fallita");

    int data = 42;
    topic.publish(data);

    // Aspetta che i work vengano processati: flush della coda dedicata e di sistema
    k_work_queue_drain(&wq, false); // processa tutti i work della nostra coda
    // Per il system wq, possiamo fare un piccolo sleep e lasciar processare
    k_sleep(K_MSEC(100));

    zassert_true(fake_work_item1.executed, "workitem 1 non eseguito");
    zassert_true(fake_work_item2.executed, "workitem 2 non eseguito");

    // seconda chiamata
    data = 42;
    topic.publish(data);

    // Aspetta che i work vengano processati: flush della coda dedicata e di sistema
    k_work_queue_drain(&wq, false); // processa tutti i work della nostra coda
    // Per il system wq, possiamo fare un piccolo sleep e lasciar processare
    k_sleep(K_MSEC(100));

    zassert_equal(fake_work_item1.call_count, 2, "workitem 1 non è stato eseguito 2 volte");
    zassert_equal(fake_work_item2.call_count, 2, "workitem 2 non è stato eseguito 2 volte");
}