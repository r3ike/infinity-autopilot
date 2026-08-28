// loggable.cpp
#include "Loggable.hpp"
#include <zephyr/logging/log.h>
#include <cstdio>

LOG_LEVEL_SET(LOG_LEVEL_DBG); 

bool Loggable::init_logging(const struct log_source_const_data* inst) {
    log_inst = inst;
    return log_inst != nullptr;
}

// vsnprintk + "%s": l'unico modo di mantenere un'API con "..." (vedi spiegazione
// precedente sul perché LOG_INST_x non accetta va_list). Qui però log_inst è
// una vera istanza registrata: il filtro runtime per-istanza funziona davvero.
#define LOG_INST_IMPL(level_macro)                          \
    char buf[128];                                          \
    va_list args; va_start(args, fmt);                       \
    vsnprintk(buf, sizeof(buf), fmt, args);                  \
    va_end(args);                                            \
    if (log_inst) { level_macro(log_inst, "%s", buf); }

void Loggable::log_dbg(const char* fmt, ...) { LOG_INST_IMPL(LOG_INST_DBG) }
void Loggable::log_inf(const char* fmt, ...) { LOG_INST_IMPL(LOG_INST_INF) }
void Loggable::log_wrn(const char* fmt, ...) { LOG_INST_IMPL(LOG_INST_WRN) }
void Loggable::log_err(const char* fmt, ...) { LOG_INST_IMPL(LOG_INST_ERR) }