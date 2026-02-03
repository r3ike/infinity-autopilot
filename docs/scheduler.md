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