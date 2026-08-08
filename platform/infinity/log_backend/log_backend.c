#include <zephyr/logging/log_backend.h>
#include "log_backend.h" 

/* Dichiarazione della funzione C++ wrapper */
extern void srimb_log_publish(void *ctx, const struct LogMessage *msg, uint64_t timestamp);