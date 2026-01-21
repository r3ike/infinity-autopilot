

# 🧠 RIASSUNTO COMPLETO — Scheduler per firmware UAV custom

## 🎯 Contesto di partenza

* Firmware **UAV custom**, non basato su PX4 / ArduPilot
* Stesso codice deve girare:

  * su **PC (SITL)**
  * su **microcontrollore (Teensy 4.1)**
* Obiettivi:

  * determinismo
  * stabilità del controllo
  * testabilità
  * semplicità architetturale
* Vincolo chiave:

  * **un solo micro**
  * **niente RTOS (almeno inizialmente)**

---

## 1️⃣ RTOS vs scheduler deterministico

### 🔹 Verità fondamentale

> **Un RTOS NON è necessario per un drone autonomo**
> e **spesso è controproducente** per il controllo di volo.

### Perché NON serve un RTOS

* Il controllo di volo richiede:

  * tempi **fissi**
  * jitter **minimo**
  * comportamento **riproducibile**
* Un RTOS:

  * introduce context switch
  * introduce jitter
  * introduce race condition
  * complica il debug
* Un **scheduler deterministico**:

  * è più semplice
  * è più affidabile
  * è più adatto a SITL

👉 **Conclusione**: puoi (e dovresti) farne a meno.

---

## 2️⃣ Cos’è davvero il “real-time” in un UAV

### ❗ Concetto chiave

> Real-time ≠ “va veloce”
> Real-time = **so esattamente QUANDO viene eseguito**

Per un UAV conta:

* **quando** gira il controllo
* **quanto dura** (WCET)
* **quanto jitter introduce**

---

## 3️⃣ Tipi di scheduler deterministici (tutte le opzioni viste)

### 🔹 1. Superloop time-driven

* Un loop principale
* Task chiamati a frequenze fisse
* Semplice, deterministico
* Poco scalabile

---

### 🔹 2. Cyclic Executive (industriale)

* Major cycle + minor cycle
* Schedule deciso **offline**
* Task assegnati a slot temporali
* Usato in avionica safety-critical

---

### 🔹 3. Time-Triggered Scheduler (task table)

* Evoluzione moderna del cyclic executive
* Tabella di task + WCET
* Ottimo compromesso tra rigore e flessibilità

---

### 🔹 4. Dual-Loop deterministico (**BEST PRACTICE**)

👉 **È la soluzione consigliata per un drone reale**

* **Hard loop** → controllo critico
* **Soft loop** → servizi lenti
* Separazione netta delle responsabilità

---

## 4️⃣ Hard loop vs Soft loop (concetto chiave)

### 🔹 Hard loop

* Gira:

  * in **interrupt (IRQ)**
  * a frequenza fissa (es. 1 kHz)
* Contiene:

  * lettura IMU
  * EKF / stima
  * controllo
  * update motori
* Regole:

  * WCET noto
  * NO I/O lenti
  * NO allocazioni
  * NO logging
  * NO blocchi

👉 **È il cuore del drone**

---

### 🔹 Soft loop

* Gira:

  * nel `main()`
  * cooperativamente
* Contiene:

  * GPS
  * logging SD
  * telemetria
  * parsing
  * health monitor
* Regole:

  * può essere lento
  * può essere interrotto
  * deve lavorare a piccoli chunk
  * **non deve mai disabilitare interrupt a lungo**

---

## 5️⃣ Perché il soft loop NON blocca l’hard loop

### Concetto cruciale

> **Solo un interrupt può preemptare la CPU**

Architettura corretta:

```
Timer IRQ → HARD LOOP (ISR)
Main()    → SOFT LOOP
```

* Se il soft loop sta girando:

  * arriva l’IRQ
  * la CPU interrompe il soft
  * esegue l’hard
  * ritorna al soft

👉 **Il soft loop non può bloccare l’hard loop**, se:

* l’hard gira in ISR
* gli interrupt non vengono disabilitati male

---

## 6️⃣ Cos’è un IRQ (Interrupt Request)

* Evento hardware che:

  * interrompe immediatamente il codice in esecuzione
  * salta a una ISR
* Su Cortex-M:

  * è gestito dall’NVIC
  * ha priorità
* È l’unico vero meccanismo di preemption senza RTOS

---

## 7️⃣ Comunicazione tra hard e soft loop

### Problema

* Hard e soft condividono dati
* Devono farlo **senza blocchi**

### Soluzioni corrette

* `volatile`
* variabili atomiche
* ring buffer lock-free
* sezioni critiche **brevissime**

### Regole

* Hard → scrive
* Soft → legge
* Mai il contrario in modo non controllato

---

## 8️⃣ GPS: come gestire un sensore lento

### ❌ Cosa NON fare

* leggere GPS nel hard loop
* aspettare fix
* parsing pesante

### ✅ Pattern corretto

* GPS gira nel soft loop o via interrupt UART
* Ogni misura:

  * viene timestampata
  * messa in buffer
* L’EKF:

  * usa l’ultimo dato disponibile
  * **senza mai aspettare**

👉 Il GPS **non comanda il controllo**

---

## 9️⃣ Logging su SD: problema serio (e soluzione)

### Problema

* SD è lenta
* non deterministica
* può bloccare per ms

### Soluzione corretta

**Buffering asincrono**

```
Hard loop → RAM ring buffer → Soft loop → SD
```

Regole:

* hard loop scrive solo in RAM (O(1))
* soft loop svuota quando può
* se la SD rallenta:

  * perdi log
  * **non perdi controllo**

---

## 🔟 WCET (Worst-Case Execution Time)

### Cos’è

* Tempo massimo garantito di esecuzione di un task

### Perché è fondamentale

* Se:

  ```
  WCET(hard loop) ≥ periodo
  ```

  👉 perdi interrupt
  👉 perdi controllo
  👉 drone instabile

### Best practice

* misurare
* profilare
* lasciare margine (≥ 30%)

---

## 1️⃣1️⃣ Determinismo ≠ tutto sincrono

Concetto spesso frainteso:

> Determinismo significa che **le parti critiche sono prevedibili**,
> non che tutto gira allo stesso tempo.

* Hard loop: deterministico
* Soft loop: best-effort
* Questa separazione **è voluta**

---

## 1️⃣2️⃣ Errori GRAVI da evitare

❌ Mettere I/O lenti nel hard loop
❌ Fare tutto nel `main()`
❌ ISR troppo lunghe
❌ Disabilitare interrupt a lungo
❌ Usare `delay()`
❌ Usare `malloc` runtime
❌ Pensare che un RTOS “risolva tutto”

---

## 1️⃣3️⃣ Architettura finale consigliata (TL;DR)

```
Timer IRQ (1 kHz)
 └─ HARD LOOP
     ├─ IMU
     ├─ Estimation
     ├─ Control
     └─ Motors

Main()
 ├─ GPS
 ├─ Logging
 ├─ Telemetry
 └─ Health
```

* Un solo core
* Nessun RTOS
* Preemption hardware
* Controllo stabile
* SITL e hardware identici concettualmente

---

## 📚 Riferimenti teorici chiave (citati)

* Kopetz — *Time-Triggered Architecture*
* Buttazzo — *Hard Real-Time Computing Systems*
* Liu & Layland — *Rate Monotonic Scheduling*
* Cyclic Executives per sistemi safety-critical
* ARINC 653 (concetto di separazione critica / non critica)

---

## 🧩 Conclusione finale (la più importante)

> **Un buon firmware UAV non è “multithreaded”.
> È temporalmente corretto.**

Se:

* sai **quando** gira il controllo
* sai **quanto dura**
* sai **cosa può interromperlo**

👉 allora stai progettando come un ingegnere avionico, non come un programmatore qualsiasi.

