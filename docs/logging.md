# Logging

## Logging EKF

L'implementazione di un **Error-State Kalman Filter (ES-EKF)** con gestione del **Delayed State** (osservazioni in ritardo, tipico del GPS) è uno degli scenari più complessi da debuggare. Se non logghi le variabili giuste, capirai solo che il drone "vola male", ma non saprai se è colpa del delay, della varianza troppo bassa, o del modello dinamico sbagliato.

Per un EKF di questo livello, devi strutturare il logging in **3 categorie logiche**.

Ecco i messaggi esatti che ti servono (in C packed).

---

### 1. LOG_EKF_STATE (Output Nominal & Bias)

Questo è lo stato "credenza attuale" del filtro. Serve per vedere la traiettoria e, soprattutto, come si evolvono i **Bias** dei sensori.

* **Frequenza:** Alta (es. 100Hz - 400Hz). Non serve a 1kHz (troppi dati), ma deve essere abbastanza veloce da vedere le reazioni agli urti.

```c
typedef struct __attribute__((packed)) {
    uint8_t  sync;          // 0xA5
    uint8_t  msg_id;        // LOG_ID_EKF_STATE
    uint32_t timestamp;     // Ora attuale (filtro aggiornato)
    
    // -- Attitudine (Quaternione Nominale) --
    float    q0, q1, q2, q3; 

    // -- Velocità e Posizione (NED Frame) --
    float    vel_n, vel_e, vel_d;
    float    pos_n, pos_e, pos_d;

    // -- Bias Stimati (CRITICO) --
    // Se questi oscillano, il filtro è instabile.
    float    gyro_bias_x, gyro_bias_y, gyro_bias_z;
    float    acc_bias_x,  acc_bias_y,  acc_bias_z;

} LogEkfState_t;

```

### 2. LOG_EKF_INNOVATION (Il cuore del Delayed State)

Qui è dove gestisci il **Delayed State**. Quando arriva un dato GPS vecchio di 200ms, il tuo EKF torna indietro nel tempo (o usa un buffer di stati passati), calcola l'errore e ripropaga.

Devi loggare **l'evento di correzione**, non lo stato storico intero. Questo messaggio viene generato **solo quando avviene una fusione** (es. arriva dato GPS o Baro).

* **Frequenza:** Bassa/Event-based (es. 5Hz Baro, 10Hz GPS).

```c
typedef struct __attribute__((packed)) {
    uint8_t  sync;
    uint8_t  msg_id;        // LOG_ID_EKF_INNOV
    uint32_t timestamp_log; // Quando stiamo scrivendo il log (NOW)
    
    uint8_t  sensor_id;     // 0=GPS_VEL, 1=GPS_POS, 2=BARO, 3=MAG
    
    // -- Gestione Delay --
    // Fondamentale: differenza tra "adesso" e "quando è stata fatta la misura"
    uint32_t timestamp_meas; // Il timestamp originale del dato sensore (OLD)
    float    dt_delay;       // Ritardo calcolato (es. 0.2s)

    // -- Matematica dell'Update --
    float    observation;    // Cosa dice il sensore (z)
    float    predicted;      // Cosa pensava l'EKF a quel tempo (h(x))
    
    float    innovation;     // z - h(x) (Residuale)
    float    innovation_var; // S (Varianza dell'innovazione: H*P*H' + R)
    
    float    test_ratio;     // NIS (Normalized Innovation Squared)
                             // Serve per il Gating: se > 1.0 (o soglia), il dato viene rigettato.
    uint8_t  flags;          // Bit 0: REJECTED, Bit 1: FUSED, Bit 2: HEALTHY
} LogEkfInnov_t;

```

**Perché questo messaggio ti salva la vita:**
Se il drone fa "toilet bowl" (gira in cerchio in hovering), guardi `test_ratio` e `innovation`. Se `test_ratio` è basso ma l'innovazione è alta, significa che la tua matrice di covarianza (P) è troppo sicura di sé e non "crede" al GPS. Se vedi `dt_delay` instabile, hai problemi nel driver del GPS.

### 3. LOG_EKF_COVARIANCE (Health del Filtro)

L'ES-EKF stima l'errore. La matrice **P** (Covarianza dell'Errore) ti dice quanto il filtro è "incerto".
La matrice intera (spesso 15x15 o 24x24) è troppo grande da loggare. Logga solo le **Diagonali**.

* **Frequenza:** Bassa (es. 10Hz - 25Hz). Le covarianze cambiano lentamente (convergono).

```c
typedef struct __attribute__((packed)) {
    uint8_t  sync;
    uint8_t  msg_id;        // LOG_ID_EKF_COV
    uint32_t timestamp;

    // -- Diagonali della matrice P --
    // Usa float scientifici se converti in testo, ma qui sono float binari.
    // Rappresentano la varianza (sigma^2) dell'errore.
    
    float    var_ang_x, var_ang_y, var_ang_z; // Errore attitudine
    float    var_vel_x, var_vel_y, var_vel_z; // Errore velocità
    float    var_pos_x, var_pos_y, var_pos_z; // Errore posizione
    
    float    var_d_bg; // Varianza delta Bias Gyro (spesso basta la magnitudine somma)
    float    var_d_ba; // Varianza delta Bias Accel

} LogEkfCov_t;

```

### 4. LOG_EKF_RESET (Il "Kick" dell'Error State)

Nell'ES-EKF, accumuli l'errore nel vettore `delta_x` e poi lo "inietti" nello stato nominale (Reset). Spesso questo avviene continuamente, ma se hai logica di reset speciali (es. reset della Yaw quando usi il magnetometro, o reset della posizione al decollo), devi loggarlo.

```c
typedef struct __attribute__((packed)) {
    uint8_t  sync;
    uint8_t  msg_id;    // LOG_ID_EKF_RESET
    uint32_t timestamp;
    uint8_t  reset_type; // 0=YAW_ALIGN, 1=POS_RESET, 2=VEL_RESET
    float    val_before;
    float    val_after;
} LogEkfReset_t;

```

---

### Strategia di Visualizzazione (Debugging)

Quando avrai questi dati su PC, ecco cosa devi cercare per validare il tuo EKF:

1. **Analisi Delay:** Grafica `LogEkfInnov.innovation` per il GPS. Se vedi dei picchi periodici, significa che il tuo `dt_delay` (compensazione ritardo) è sbagliato. L'innovazione dovrebbe essere Rumore Bianco (media zero, casuale). Se vedi "onde", stai compensando male il tempo.
2. **Convergenza Bias:** Grafica `LogEkfState.gyro_bias`. All'accensione devono muoversi e poi stabilizzarsi su una linea piatta. Se continuano a vagare, il guadagno del filtro (process noise Q per i bias) è troppo alto.
3. **Consistenza (Consistency Check):** Grafica l'**Innovazione** sovrapposta a **+/- 2*SQRT(Innovation_Var)** (i confini a 2 sigma).
* Se l'innovazione esce quasi sempre dai confini (linee tratteggiate), la tua matrice **R** (rumore sensore) è troppo ottimistica (troppo bassa) o **P** è troppo piccola.
* -> Questo grafico sarà il tuo strumento principale di tuning.

