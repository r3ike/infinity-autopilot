# Guida al Contributo - Infinity Autopilot

Innanzitutto, grazie per aver considerato di contribuire a Infinity Autopilot! È grazie a sviluppatori come te che questo progetto continua a migliorare.

## Sommario

- [Come Posso Contribuire?](#come-posso-contribuire)
- [Procedure e Processi](#procedure-e-processi)
- [Linee Guida per le Issue](#linee-guida-per-le-issue)
- [Linee Guida per le Pull Request](#linee-guida-per-le-pull-request)
- [Stile del Codice](#stile-del-codice)
- [Compilazione e Test](#compilazione-e-test)
- [Processo di Review](#processo-di-review)
- [Istruzioni di Rilascio](#istruzioni-di-rilascio)

---

## Come Posso Contribuire?

### Segnalare Bug
- Controlla prima se il bug è già stato segnalato nelle [issue aperte](https://github.com/reike94/infinity-autopilot/issues)
- Se è un bug nuovo, usa il template "Bug Report"
- Includi tutti i dettagli: OS, versione, hardware, step di riproduzione, log
- Se possibile, fornisci un minimal reproducible example

### Suggerire Enhancement
- Usa il template "Feature Request"
- Descrivi chiaramente il problema e la soluzione proposta
- Considera l'impatto sul progetto
- Cerca feedback della comunità prima di implementare

### Migliorare la Documentazione
- Documentazione nel README
- Docstring nel codice
- Guide wiki
- Commenti per codice complesso

### Implementare Feature o Fix
- Assicurati che sia assegnato a te o coordinati tramite issue
- Crea un branch dal branch di sviluppo appropriato
- Segui le linee guida di stile
- Scrivi test
- Crea una PR con descrizione dettagliata

---

## Procedure e Processi

### Forking e Setup Locale

```bash
# 1. Fai un fork del repository
# 2. Clona il tuo fork
git clone https://github.com/YOUR-USERNAME/infinity-autopilot.git
cd infinity-autopilot

# 3. Aggiungi il repository originale come upstream
git remote add upstream https://github.com/reike/infinity-autopilot.git

# 4. Sincronizza il tuo fork
git fetch upstream
git checkout main
git merge upstream/main
```

### Inizializzazione dell'Ambiente

```bash
# Installa dipendenze
sudo apt-get install cmake gcc-arm-none-eabi

# Inizializza i moduli Zephyr (se applicabile)
west init -m https://github.com/zephyrproject-rtos/zephyr.git

# Installa dipendenze Python
pip install -r requirements.txt  # se presente
```

### Naming Convention per i Branch

```
stable                    # Branch di rilascio stabile
main                 # Branch di sviluppo
feature/DESCRIPTION     # Nuove feature (es: feature/gps-integration)
fix/DESCRIPTION      # Bug fix (es: bugfix/sensor-calibration)
docs/DESCRIPTION        # Documentazione (es: docs/api-guide)
refactor/DESCRIPTION    # Refactoring (es: refactor/scheduler-cleanup)
```

---

## Linee Guida per le Issue

### Titoli Descrittivi
```
BUONO:   [SENSOR] Bug gyro reading oscillation at startup
CATTIVO: Bug
BUONO:   [FEATURE] Add magnetometer calibration wizard
CATTIVO: New thing
```

### Prima di Aprire una Issue
- [ ] Hai cercato issue simili?
- [ ] Stai usando l'ultima versione?
- [ ] Hai incluso tutti i dettagli richiesti dal template?
- [ ] Hai provato i step di debug base?

### Dopo Aprire una Issue
- Rispondi alle domande di chiarimento
- Fornisci ulteriori test/log se richiesti
- Aggiungi label appropriata se hai permessi
- Considera di risolvere tu stesso se semplice

---

## Linee Guida per le Pull Request

### Prima di Creare una PR
1. **Sincronizza con upstream**
   ```bash
   git fetch upstream
   git rebase upstream/develop
   ```

2. **Crea un branch descrittivo**
   ```bash
   git checkout -b feature/my-feature
   ```

3. **Scrivi codice testato**
   - Aggiungi/aggiorna test
   - Verifica la compilazione
   - Esegui test locali

4. **Scrivi un commit message chiaro seguendo i conventional commit**
  

### Sottomissione della PR
- Usa il template fornito
- Linka l'issue correlata con "Fixes #123"
- Descrivi cosa cambia e perché
- Includi screenshot/video se rilevante
- Assicurati che tutte le checkbox siano completate

### Dopo la Sottomissione
- Rispondi ai commenti del reviewer
- Richiedi re-review dopo le modifiche
- Non force-push dopo le richieste di review (se possibile)
- Mantieni pulita la storia dei commit

---

## Stile del Codice

### C++
```cpp
// Nomi delle classi: PascalCase
class FlightController {
private:
    // Membri privati: camelCase con underscore finale
    float altitude_;
    
    // Metodi privati: camelCase
    void updateSensors();
    
public:
    // Metodi pubblici: camelCase
    void initialize();
    bool isReady() const;
    
    // Costanti: UPPER_SNAKE_CASE
    static constexpr float MAX_TILT_ANGLE = 45.0f;
};

// Funzioni globali: camelCase
void logError(const char* message);

// Costanti globali: UPPER_SNAKE_CASE
constexpr int BUFFER_SIZE = 256;

// Variabili globali: g_ prefix + camelCase (evita se possibile)
static int g_errorCount = 0;
```

**Linee Guida Generali:**
- Larghezza linea: 100 caratteri
- Indentazione: 4 spazi (NO tab)
- Utilizza `const` e `constexpr` dove appropriato
- Usa smart pointer dove possibile
- Evita macro globali
- Commenta logica non ovvia

### Python
```python
# Segui PEP 8
class SensorReader:
    """Legge dati dai sensori."""
    
    def __init__(self):
        self.sensors = []
    
    def read_data(self) -> dict:
        """Legge i dati."""
        pass
```

### CMake
```cmake
# Usa lowercase per i target
add_executable(flight_controller main.cpp)

# Dichiara dipendenze esplicite
target_link_libraries(flight_controller PRIVATE zephyr_interface)

# Commenta le decisioni non ovvie
```

---

## Compilazione e Test

### Build Locale

```bash
# Build per SITL
west build -b native_sim -d build

# Build per Teensy 4.1
west build -b infinity_fc -d build_hw

# Build pulita
rm -rf build && west build -b native_sim -d build
```

### Test Locale

```bash
# Se presente test framework
cd build
ctest

# O manualmente
./test_runner
```

### Verifiche Pre-Commit
```bash
# Controlla build
cmake --build build -- -j$(nproc)

# Controlla test
cmake --build build --target test

# Verifiche di stile (se configurato)
# clang-format, clang-tidy, etc.
```

### GitHub Actions
- Tutti i test devono passare su GitHub Actions
- Commits devono compilare su tutte le piattaforme target
- Nessun warning di compilazione

---

## Processo di Review

### Da Parte del Reviewer
- Commenti costruttivi e gentili
- Approvazione quando tutto è in ordine
- Request changes se necessario
- Merging quando approvato

### Risposte ai Commenti
- Rispondi punto per punto
- Chiedi chiarimenti se non capite
- Esegui gli aggiustamenti richiesti
- Segna come risolti i commenti dopo le modifiche

### Cosa Fa Approvare una PR
- Codice segue le linee guida di stile
- Test appropriati aggiunti
- Documentazione aggiornata
- Nessun breaking change non dichiarato
- Build passa su tutte le piattaforme
- Nessuna dipendenza non necessaria

---

## Istruzioni di Rilascio

*Solo per maintainer*

### Versioning
Seguiamo [Semantic Versioning](https://semver.org/):
- **MAJOR**: Breaking changes
- **MINOR**: Nuove feature compatibili
- **PATCH**: Bug fix

### Processo di Release
1. Aggiorna version in `CMakeLists.txt`
2. Aggiorna `CHANGELOG.md`
3. Crea tag: `git tag -a v1.2.3 -m "Release 1.2.3"`
4. Push: `git push upstream v1.2.3`
5. Crea GitHub Release con note

---

## Domande?

- 📖 Consulta la [Wiki](https://github.com/reike/infinity-autopilot/wiki)
- 💬 Apri una [Discussion](https://github.com/reike/infinity-autopilot/discussions)
- 📧 Contatta i maintainer
- 🐛 Segnala un bug se qualcosa non è chiaro

---

## Licenza

Contribuendo a questo progetto, accetti che i tuoi contributi siano concessi in licenza secondo i termini della licenza del progetto.

Grazie per aver contribuito! 🚀
