#pragma once 
#include <cstdint>

namespace srimb
{

    /**
     * TODO:
     *  - aggiungere subscriber name
     */

struct WorkItemCallback
{
    WorkItemBase* work_item {nullptr};
    WorkQueue* work_queue {nullptr};
    uint8_t required_updates{1};        // numero di update richiesti per il submit del workitem
    uint8_t updates_count{0};
};

class SRIMBSub
{
private:
    uint64_t last_generation_{0};

    bool missed_ {false}; // flag to warn about a missed message in the queue topic
public:
    SRIMBSub(){}
    ~SRIMBSub() = default;
    
    void set_last_generation(uint64_t gen) {last_generation_ = gen;}
    uint64_t get_last_generation() {return last_generation_;}
    
    void set_missed(bool missed) {missed_ = missed;}
    bool get_missed() {return missed_;}

};

} // namespace srimb


