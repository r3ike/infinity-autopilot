# **Infinity autopilot**
Infinity è un autopilot avanzato totalmente open-source per UAV , permette compatibilità tra vari sistemi grazie ad un **Hardware Abstraction Layer** il quale permette attraverso vari environment diversi di essere compilato per vari sistemi e inoltre permette testing in **SITL**.


> [!WARNING]
> There is currently **no stable release** of **Infinity-Autopilot** available.  
> Il progetto è ancora in una fare **Iniziale** di sviluppo, le varie funzionalità saranno implementate nel prossimo periodo.


## Features

- EKF Error-state con delayed state
- Controller PID
- Controller LQR
- Controller MPC
- Logging avanzato
- Simulazione SITL


## Come compilare il progetto per la prima volta

Segui questi passaggi per impostare l'ambiente di sviluppo e compilare il firmware.

### Prerequisiti

Assicurati di aver installato i prerequisiti di Zephyr RTOS: Python, CMake, Ninja, e il toolchain per il tuo target (es. ARM GCC). Segui la [Guida introduttiva ufficiale di Zephyr](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) fino al punto in cui si installano i pacchetti Python di base, **ma fermati prima di creare un nuovo workspace con `west init`** (lo faremo dopo con il nostro repository).

### Passaggi

1.  **Clona il repository:**
    ```bash
    git clone https://github.com/tuo-username/infinity-autopilot.git
    cd infinity-autopilot
    ```

2.  **Inizializza il workspace West:**
    Poiché il repository è già un workspace, devi inizializzare west puntando al manifest locale.
    ```bash
    west init -l .
    ```
    Questo comando legge il file `west.yml` nella directory corrente e configura west per usarlo come manifest.

3.  **Scarica i moduli di Zephyr:**
    ```bash
    west update
    ```
    Questo comando scaricherà il kernel Zephyr (`zephyr/`), gli HAL (`modules/`) e tutti gli altri moduli necessari, creando la struttura completa del workspace.

4.  **Esporta il percorso di Zephyr (opzionale ma consigliato):**
    ```bash
    west zephyr-export
    ```

5.  **Attiva l'ambiente Python (se usi un virtual environment):**
    Se hai installato i prerequisiti in un virtual environment (come raccomandato da Zephyr), attivalo:
    ```bash
    source venv/bin/activate  # O il percorso del tuo venv
    ```

6.  **Sorgente le variabili d'ambiente di Zephyr:**
    ```bash
    source zephyr/zephyr-env.sh
    ```

7.  **Compila l'applicazione:**
    Ora puoi compilare il progetto normalmente. Poiché la tua applicazione è nella radice, il comando è semplicemente:
    ```bash
    west build -b <board_name> .
    ```
    (Sostituisci `<board_name>` con la tua board, es. `mio_drone_sim` per SITL o `mio_drone` per hardware).

8.  **Flash (se su hardware reale):**
    ```bash
    west flash
    ```
