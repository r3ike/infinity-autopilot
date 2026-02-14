#pragma once
#include <stdint.h>

inline uint32_t hz_to_ms(uint32_t value_hz){
    return (1 / value_hz) * 1000;
}

inline uint32_t hz_to_us(uint32_t value_hz){
    return (1 / value_hz) * 1000000;
}