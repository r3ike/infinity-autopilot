<h1 align="center">
    Infinity-Autopilot
</h1>

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


## Development setup

Segui questi passaggi per impostare l'ambiente di sviluppo e compilare il firmware.

### Prerequisites (Ubuntu 22.04)
```bash
sudo apt update
sudo apt upgrade

```
#### Zephyr installation
L'installazione di zephyr è obbligatoria e si ocnsiglia di effetturala seguendo la [guida 
ufficiale](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

#### Gazebo installation (optional)
Se si è interessati ad effettuare simulazioni SITL è necessaria l'installazione di gazebo
attraverso la [guida ufficiale](https://gazebosim.org/docs/all/getstarted/).

### Create and activate a virtual environment
```bash
python3 -m venv --system-site-packages venv
source venv/bin/activate

pip install -r zephyr/scripts/requirements.txt
pip install -e .[dev]
```


### Passaggi

1.  **Clona il repository:**
    ```bash
    git clone https://github.com/r3ike/infinity-autopilot.git
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

5.  **Sorgente le variabili d'ambiente di Zephyr:**
    ```bash
    source zephyr/zephyr-env.sh
    ```

6.  **Compila l'applicazione:**
    Ora puoi compilare il progetto normalmente. Poiché la tua applicazione è nella radice, il comando è semplicemente:
    ```bash
    west build -b <board_name> .
    ```
    (Sostituisci `<board_name>` con la tua board, es. `mio_drone_sim` per SITL o `mio_drone` per hardware).

7.  **Flash (se su hardware reale):**
    ```bash
    west flash
    ```
