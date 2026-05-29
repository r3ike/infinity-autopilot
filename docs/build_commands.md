## Prepare venv python
```bash
pip install -r zephyr/scripts/requirements.txt
```

## Build
### Build hw
```bash
west build -b teensy41 . -- -DTARGET_PLATFORM=TEENSY41
```

### Build sitl
```bash
west build -b native_sim/native/64  . -- -DTARGET_PLATFORM=SITL
```

## Change kconfig
Questo comando bisogna eseguirlo dopo aver eseguito la prima build.
```bash
west build -t menuconfig
```
Una volta modificate le configurazione interessate è necessario eseguire nuovamente il comando di build.

## Clean build
```bash
west build -t clean
```