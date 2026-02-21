# components/arduino_core/CMakeLists.txt
# Componente per il core Teensyduino 4.1
# Questo componente fornisce l'implementazione completa del core Teensy 4.1
# come libreria statica, con tutti i flag e le definizioni necessarie.

# 1. DEFINIZIONE DEI PERCORSI
# ============================
# La directory contenente i file del core (copia dalla installazione Teensyduino)
set(CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Directory con header aggiuntivi
set(AVR_COMPAT_PATH ${CORE_PATH}/avr)
set(DEBUG_PATH ${CORE_PATH}/debug)

# Il linker script specifico per Teensy 4.1
set(LINKER_SCRIPT ${CORE_PATH}/imxrt1062_t41.ld)

# Verifica che i percorsi critici esistano
if(NOT EXISTS ${CORE_PATH})
    message(FATAL_ERROR "CORE_PATH ${CORE_PATH} non esiste!")
endif()

if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "LINKER_SCRIPT ${LINKER_SCRIPT} non trovato!")
endif()

# 2. RACCOLTA DEI SORGENTI
# =========================
# Elenco COMPLETO dei file sorgente del core Teensy 4.1
# Tutti i file .c e .cpp nella directory del core
set(CORE_SOURCES
    # File principali del core
    ${CORE_PATH}/main.cpp
    ${CORE_PATH}/yield.cpp
    ${CORE_PATH}/startup.c
    ${CORE_PATH}/nonstd.c
    
    # Gestione pin e I/O
    ${CORE_PATH}/digital.c
    ${CORE_PATH}/analog.c
    ${CORE_PATH}/pwm.c
    ${CORE_PATH}/interrupt.c
    
    # Timing e ritardi
    ${CORE_PATH}/delay.c
    
    # Comunicazione seriale
    ${CORE_PATH}/HardwareSerial.cpp
    ${CORE_PATH}/HardwareSerial1.cpp
    ${CORE_PATH}/HardwareSerial2.cpp
    ${CORE_PATH}/HardwareSerial3.cpp
    ${CORE_PATH}/HardwareSerial4.cpp
    ${CORE_PATH}/HardwareSerial5.cpp
    ${CORE_PATH}/HardwareSerial6.cpp
    ${CORE_PATH}/HardwareSerial7.cpp
    ${CORE_PATH}/HardwareSerial8.cpp
    ${CORE_PATH}/serialEvent.cpp
    ${CORE_PATH}/serialEvent1.cpp
    ${CORE_PATH}/serialEvent2.cpp
    ${CORE_PATH}/serialEvent3.cpp
    ${CORE_PATH}/serialEvent4.cpp
    ${CORE_PATH}/serialEvent5.cpp
    ${CORE_PATH}/serialEvent6.cpp
    ${CORE_PATH}/serialEvent7.cpp
    ${CORE_PATH}/serialEvent8.cpp
    ${CORE_PATH}/serialEventUSB1.cpp
    ${CORE_PATH}/serialEventUSB2.cpp
    
    # USB
    ${CORE_PATH}/usb.c
    ${CORE_PATH}/usb_inst.cpp
    ${CORE_PATH}/usb_desc.c
    ${CORE_PATH}/usb_serial.c
    ${CORE_PATH}/usb_seremu.c
    ${CORE_PATH}/usb_keyboard.c
    ${CORE_PATH}/usb_mouse.c
    ${CORE_PATH}/usb_joystick.c
    ${CORE_PATH}/usb_midi.c
    ${CORE_PATH}/usb_audio.cpp
    ${CORE_PATH}/usb_rawhid.c
    ${CORE_PATH}/usb_flightsim.cpp
    ${CORE_PATH}/usb_mtp.c
    ${CORE_PATH}/usb_touch.c
    
    # Print e Stream
    ${CORE_PATH}/Print.cpp
    ${CORE_PATH}/Stream.cpp
    
    # I/O avanzato
    ${CORE_PATH}/DMAChannel.cpp
    ${CORE_PATH}/IntervalTimer.cpp
    ${CORE_PATH}/EventResponder.cpp
    
    # Audio e MTP
    ${CORE_PATH}/AudioStream.cpp
    ${CORE_PATH}/MTP_Storage.cpp
    ${CORE_PATH}/MTP_Teensy.cpp
    
    # Utility
    ${CORE_PATH}/IPAddress.cpp
    ${CORE_PATH}/Time.cpp
    ${CORE_PATH}/Tone.cpp
    ${CORE_PATH}/WString.cpp
    ${CORE_PATH}/WMath.cpp
    ${CORE_PATH}/new.cpp
    ${CORE_PATH}/CrashReport.cpp
    
    # Memoria e allocazione
    ${CORE_PATH}/libc.c
    ${CORE_PATH}/sm_malloc.c
    ${CORE_PATH}/sm_calloc.c
    ${CORE_PATH}/sm_realloc.c
    ${CORE_PATH}/sm_realloc_i.c
    ${CORE_PATH}/sm_realloc_move.c
    ${CORE_PATH}/sm_free.c
    ${CORE_PATH}/sm_zalloc.c
    ${CORE_PATH}/sm_szalloc.c
    ${CORE_PATH}/sm_pool.c
    ${CORE_PATH}/sm_hash.c
    ${CORE_PATH}/sm_alloc_valid.c
    ${CORE_PATH}/sm_util.c
    ${CORE_PATH}/sm_malloc_stats.c
    
    # EEPROM e memoria
    ${CORE_PATH}/eeprom.c
    ${CORE_PATH}/extmem.c
    
    # Orologio e configurazione
    ${CORE_PATH}/rtc.c
    ${CORE_PATH}/clockspeed.c
    ${CORE_PATH}/tempmon.c
    
    # Boot e fuse
    ${CORE_PATH}/bootdata.c
    ${CORE_PATH}/fuse.c
    
    # Debug e diagnostica
   # ${CORE_PATH}/debug/printf.c
    
    # Tastiera
    ${CORE_PATH}/keylayouts.c
)

# Verifica che tutti i file sorgente esistano
foreach(source ${CORE_SOURCES})
    if(NOT EXISTS ${source})
        message(WARNING "File sorgente mancante: ${source}")
    endif()
endforeach()

# Opzionale: usare GLOB per raccogliere automaticamente i file (sconsigliato per progetti seri)
# file(GLOB CORE_SOURCES ${CORE_PATH}/*.c ${CORE_PATH}/*.cpp)

# 3. TARGET PER I FLAG COMUNI (INTERFACE)
# ========================================
# Questo target INTERFACE propaga flag e definizioni a chiunque dipenda da arduino_core
add_library(arduino_flags INTERFACE)

# Flag di compilazione specifici per Teensy 4.1
# ARM Cortex-M7 con FPU (floating point unit) double-precision
# -mcpu=cortex-m7: processore target
# -mfloat-abi=hard: usa FPU per floating point
# -mfpu=fpv5-d16: FPU con 16 registri double-precision
# -mthumb: usa set di istruzioni Thumb
# -ffunction-sections/-fdata-sections: sezioni separate per ogni funzione/dato (utile per linking)
target_compile_options(arduino_flags INTERFACE
    "-mcpu=cortex-m7"
    "-mfloat-abi=hard"
    "-mfpu=fpv5-d16"
    "-mthumb"
    "-ffunction-sections"
    "-fdata-sections"
    "-fno-builtin"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>"
    "-Wall"
    "-Wextra"
    "-Wno-unused-parameter"
    "-g"
    "-Os"
)

# Definizioni del preprocessore per Teensy 4.1
target_compile_definitions(arduino_flags INTERFACE
    "F_CPU=600000000"               # Frequenza CPU: 600 MHz
    "ARDUINO=10819"                 # Versione Arduino IDE
    "ARDUINO_TEENSY41"              # Identificativo board
    "ARDUINO_ARCH_IMXRT"            # Architettura ARM
    "USB_SERIAL"                    # Tipo USB predefinito
    "LAYOUT_US_ENGLISH"             # Layout tastiera
    "__IMXRT1062__"                 # MCU specifico
    "CORE_TEENSY"                   # Identificativo core
    "TEENSYDUINO=158"               # Versione Teensyduino
    "__FPU_PRESENT"                 # FPU disponibile
    "IMXRT_PACKAGE_LQFP144"         # Tipo di package
)

# Opzioni di linking
target_link_options(arduino_flags INTERFACE
    "-mcpu=cortex-m7"
    "-mfloat-abi=hard"
    "-mfpu=fpv5-d16"
    "-mthumb"
    "-Wl,--gc-sections"
    "-Wl,--cref"
    "-Wl,--print-memory-usage"
    "-Wl,-Map=${CMAKE_PROJECT_NAME}.map"
    "LINKER:-script,${LINKER_SCRIPT}"
)

# 4. LA LIBRERIA DEL CORE
# =======================
# Crea la libreria statica con tutti i sorgenti del core
add_library(arduino_core STATIC ${CORE_SOURCES})

# Il core dipende dai flag (PUBLIC perché devono propagarsi)
target_link_libraries(arduino_core PUBLIC arduino_flags)

# Directory di inclusione pubbliche
target_include_directories(arduino_core PUBLIC
    ${CORE_PATH}              # Header principali (Arduino.h, etc.)
    ${AVR_COMPAT_PATH}        # Compatibilità AVR
    ${DEBUG_PATH}             # Header di debug
)

# Dipendenze da librerie di sistema
# - m: libreria matematica
# - c: libreria C standard
target_link_libraries(arduino_core PRIVATE m c)

# Se il tuo progetto ha un componente "core" (quello che avevamo definito prima),
# e arduino_core dipende da esso, scommenta la riga seguente
# target_link_libraries(arduino_core PUBLIC core)

# 5. CONFIGURAZIONE OPZIONALE CON KCONFIG
# ========================================
# Opzionale: abilita la configurazione di alcune caratteristiche del core
# Se esiste un file Kconfig, verrà incluso automaticamente dal Kconfig radice
# Esempio di opzioni configurabili:
# - CONFIG_ARDUINO_USB_TYPE (SERIAL, MIDI, AUDIO, ecc.)
# - CONFIG_ARDUINO_OPTIMIZATION (Os, O2, ecc.)
# - CONFIG_ARDUINO_DEBUG (abilitare debug output)

# 6. MESSAGGI DI DIAGNOSTICA DETTAGLIATI
# =======================================
message(STATUS "========== Teensy 4.1 Core Configuration ==========")
message(STATUS "Core Path:        ${CORE_PATH}")
message(STATUS "Linker Script:    ${LINKER_SCRIPT}")
message(STATUS "AVR Compat:       ${AVR_COMPAT_PATH}")
message(STATUS "Debug Path:       ${DEBUG_PATH}")
message(STATUS "Total Sources:    ${CORE_SOURCES}")
message(STATUS "===================================================")

# Conteggio dei file sorgente per diagnostica
list(LENGTH CORE_SOURCES CORE_SOURCE_COUNT)
message(STATUS "Number of source files: ${CORE_SOURCE_COUNT}")