# Infinity autopilot
Infinity è un firmware opensource per quadricotteri autonomi.


## Scheduler

**Comparazione Pratica Diretta tra i 4 approcci principali:**

## **1. TIME-TRIGGERED SEMPLICE (Polling Loop)**

```cpp
// APPROCCIO: Loop fisso con controllo tempo
void loop() {
    uint32_t now = micros();
    
    // Ogni 2ms (500Hz) - Controllo volo
    if(now - last_control >= 2000) {
        readIMU();
        pidController();
        outputMotors();
        last_control = now;
    }
    
    // Ogni 10ms (100Hz) - Navigazione
    if(now - last_nav >= 10000) {
        updateNavigation();
        last_nav = now;
    }
}
```
**✅ VANTAGGI:**
- **Semplice da implementare** (10 minuti)
- **Basso overhead** (quasi zero)
- **Deterministico** se niente interrupt
- **Debug facile** con Serial.print

**❌ SVANTAGGI UAV:**
- **Jitter alto** (fino a 100µs su Teensy)
- **Nessuna priorità** (task lunghi bloccano tutto)
- **CPU spreca tempo** in attesa
- **Scalabilità scarsa** (max 5-6 task)

**📊 PERFORMANCE REALI Teensy 4.1:**
- Jitter: 50-150µs @ 500Hz
- CPU usage: 60-80% (inefficiente)
- Max task: ~8 prima di perdere deadline

---

## **2. SCHEDULER A TICK (Come FreeRTOS minimal)**

```cpp
// APPROCCIO: Tick a frequenza fissa
void schedulerTick() {
    static uint32_t tick = 0;
    
    // Task a tick multipli
    if(tick % 1 == 0) controlTask();  // 1kHz
    if(tick % 2 == 0) imuTask();      // 500Hz
    if(tick % 10 == 0) navTask();     // 100Hz
    
    tick++;
    delayMicroseconds(1000); // 1ms tick
}
```
**✅ VANTAGGI:**
- **Sincronizzazione naturale** tra task
- **Facile debugging** (tick contati)
- **Buono per SITL** (simulazione precisa)
- **Distribuzione carico**

**❌ SVANTAGGI UAV:**
- **Rigido** (tutti i task multipli del tick base)
- **Worst-case** se molti task sullo stesso tick
- **Overhead delay** (anche se CPU idle)
- **Non adatta a task sporadici**

**📊 PERFORMANCE REALI:**
- Jitter: 20-50µs @ 1kHz tick
- CPU: 70-90% (delay waste)
- Scalabilità: media (10-12 task)

---

## **3. RATE MONOTONIC (EDF semplice)**

```cpp
// APPROCCIO: Task con periodi propri
void rateMonotonicScheduler() {
    uint32_t now = micros();
    
    for(int i = 0; i < numTasks; i++) {
        if(now - task[i].lastRun >= task[i].period) {
            task[i].function();
            task[i].lastRun = now;
        }
    }
    
    // Attiva attiva per CPU alta
    while(micros() - now < 100) {
        idleTask(); // O NOP
    }
}
```
**✅ VANTAGGI UAV:**
- **Periodi indipendenti** (500Hz, 237Hz, ecc)
- **Priorità implicita** (task più veloci prima)
- **Utilizzo CPU efficiente** (idle quando possibile)
- **Teoria formale** (analisi di schedulabilità)

**❌ SVANTAGGI:**
- **Implementazione complessa** per priorità
- **Jitter variabile** (dipende da ordine esecuzione)
- **Task lunghi** possono bloccare task veloci
- **Analysis complessa** WCET

**📊 PERFORMANCE REALI Teensy 4.1:**
- Jitter: 10-100µs (dipende da carico)
- CPU: 40-60% (efficiente)
- Schedulabilità: verificabile matematicamente
- Max task: 15-20 ben configurati

---

## **4. FRAME SCHEDULING (Deterministico)**

```cpp
// APPROCCIO: Time slot pre-allocati
void frameScheduler() {
    uint32_t frameStart = micros();
    
    // Slot 0: 0-500µs - IMU
    imuTask();
    while(micros() - frameStart < 500); // Wait exact
    
    // Slot 1: 500-1000µs - Control
    controlTask();
    while(micros() - frameStart < 1000);
    
    // Slot 2: 1000-1500µs - Navigation
    navTask();
    while(micros() - frameStart < 1500);
}
```
**✅ VANTAGGI UAV:**
- **Massimo determinismo** (jitter < 5µs)
- **Garantito WCET** (slot fissi)
- **Isolamento task** (uno non influenza altro)
- **Ottimo per controllo** (500Hz-1kHz)

**❌ SVANTAGGI:**
- **Rigidissimo** (cambi frequenza = riprogettare)
- **CPU sprecata** (se task finisce prima)
- **Complesso debugging** (timing esatto)
- **Non adatta task sporadici**

**📊 PERFORMANCE REALI:**
- Jitter: 1-5µs (il migliore)
- CPU: spesso <50% (spreco)
- Max freq: 2kHz pratico
- Robustezza: alta (prevedibile)

---

## **5. HYBRID (Consigliato per UAV)**

```cpp
// APPROCCIO: Mix dei migliori
void hybridUAVScheduler() {
    // LIVELLO 1: Time-triggered hard real-time (frame)
    uint32_t frameStart = micros();
    
    // Slot fissi per controllo (0-1500µs)
    imuTask();      // 0-200µs
    controlTask();  // 200-800µs
    motorTask();    // 800-1500µs
    
    // LIVELLO 2: Rate monotonic per task soft (1500-9500µs)
    uint32_t softStart = micros();
    for(auto& task : softTasks) {
        if(micros() - task.lastRun >= task.period) {
            task.execute();
            task.lastRun = micros();
        }
    }
    
    // LIVELLO 3: Background se c'è tempo (ultimi 500µs)
    while(micros() - frameStart < 10000) { // 10ms frame
        backgroundTask();
    }
}
```

## **🗺️ TABELLA DECISIONALE PER UAV:**

| **CRITERIO** | **TT Semplice** | **Tick** | **Rate Monotonic** | **Frame** | **Hybrid** |
|-------------|----------------|----------|-------------------|-----------|------------|
| **Determinismo** | Basso (3/10) | Medio (6/10) | Medio-Alto (7/10) | **Alto (9/10)** | Alto (8/10) |
| **Facilità** | **Facile (9/10)** | Medio (7/10) | Difficile (4/10) | Medio (6/10) | Medio (5/10) |
| **Efficienza CPU** | Bassa (4/10) | Bassa (5/10) | **Alta (8/10)** | Bassa (3/10) | Alta (7/10) |
| **Scalabilità** | Bassa (3/10) | Media (6/10) | **Alta (8/10)** | Bassa (2/10) | Media (7/10) |
| **Debug SITL** | **Facile (9/10)** | Facile (8/10) | Difficile (5/10) | Difficile (4/10) | Medio (6/10) |
| **Jitter tipico** | 50-150µs | 20-50µs | 10-100µs | **1-5µs** | 5-20µs |
| **Per UAV?** | Solo prototipo | Demo/Test | **Produzione** | Competizione | **Custom avanzato** |

## **🎯 RACCOMANDAZIONE FINALE PRATICA:**

### **Per il TUO UAV su Teensy 4.1:**

**START CON QUESTO (settimana 1-2):**
```cpp
// Phase 1: Time-Triggered semplice per MVP
void simpleUAVLoop() {
    static uint32_t last_imu, last_ctrl, last_nav;
    uint32_t now = micros();
    
    if(now - last_imu >= 1000) {    // 1kHz IMU
        readIMU(); last_imu = now;
    }
    if(now - last_ctrl >= 2000) {   // 500Hz controllo
        attitudeControl(); last_ctrl = now;
    }
    if(now - last_nav >= 10000) {   // 100Hz nav
        updateGPS(); last_nav = now;
    }
}
```
**Perché:** Voli in 2 giorni, debugging facile, capisci i requisiti reali.

---

**EVOLVI A QUESTO (settimana 3-4):**
```cpp
// Phase 2: Rate Monotonic migliorato
class ImprovedRM {
    // Task ordinati per frequenza (alta -> priorità alta)
    Task tasks[] = {
        {imuTask, 1000},     // 1kHz - prio alta
        {controlTask, 2000}, // 500Hz
        {navTask, 10000},    // 100Hz
        {commsTask, 20000},  // 50Hz
    };
    
    void run() {
        uint32_t now = micros();
        for(int i = 0; i < 4; i++) { // Solo task critici
            if(now - tasks[i].last >= tasks[i].period) {
                tasks[i].func();
                tasks[i].last = now;
            }
        }
    }
};
```
**Perché:** Più efficiente, gestisci 10+ task, priorità naturale.

---

**SE SERVE MASSIMO DETERMINISMO (settimana 5+):**
```cpp
// Phase 3: Hybrid per competizione
void competitionScheduler() {
    // Frame da 2ms per controllo (500Hz)
    uint32_t frame = micros();
    
    // Slot fissi 0-1500µs per hard RT
    readIMU();           // deve finire in 200µs
    attitudeControl();   // 200-800µs
    motorOutput();       // 800-1500µs
    
    // Resto del frame per soft tasks
    while(micros() - frame < 2000) {
        runSoftTasks();  // navigazione, comms, ecc
    }
}
```

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

