#pragma once
#include <stdint.h>

namespace srimb
{
template<typename T>
struct SRIMB_Topic {
    T data;
    uint64_t timestamp = 0;
    uint64_t generation = 0;
};
    
} // namespace srimb


