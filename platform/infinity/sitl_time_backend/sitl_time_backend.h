#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Inizializza il timer (chiamato automaticamente da Zephyr al boot)
int sitl_timer_init(void);

// L'applicazione chiamerà questa funzione quando riceve HIL_SENSOR
void sitl_time_update(uint64_t new_time_us);

#ifdef __cplusplus
}
#endif
