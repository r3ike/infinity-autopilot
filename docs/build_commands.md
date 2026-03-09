## Build
```bash
west build -b teensy41 . -- -DTARGET_PLATFORM=TEENSY41
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