#pragma once 
#include <cstdint>

namespace srimb
{

class SRIMBSub
{
private:
    uint64_t last_generation_{0};
public:
    void set_last_generation(uint64_t gen) {last_generation_ = gen;}
    uint64_t get_last_generation() {return last_generation_;}
    virtual ~SRIMBSub() = default;
};

} // namespace srimb


