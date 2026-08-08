// log_backend.h
#pragma once

#include <stdint.h>   // per uint32_t, uint64_t
#include <stddef.h>   // per size_t

// La struttura che verrà pubblicata sul topic di log.
// Adattala alle tue esigenze (campi, dimensione testo, ecc.)
typedef struct {
    uint64_t timestamp;   // timestamp del messaggio
    uint32_t level;       // livello (es. LOG_LEVEL_INF, LOG_LEVEL_ERR)
    char     text[128];   // testo del messaggio (Zephyr di solito usa stringhe formattate)
    // Aggiungi altri campi se necessario (es. modulo, linea, etc.)
} log_msg_t;

// Prototipo della funzione wrapper, con collegamento C.
// Viene implementata in un file .cpp (vedi esempio precedente).
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Pubblica un messaggio di log sul topic SRIMB.
 * @param ctx       puntatore al topic (SRIMBTopic<log_msg_t>*)
 * @param msg       messaggio da pubblicare
 * @param timestamp timestamp (già fornito da Zephyr)
 */
void srimb_log_publish(void *ctx, const log_msg_t *msg, uint64_t timestamp);

#ifdef __cplusplus
}
#endif