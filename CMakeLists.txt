cmake_minimum_required(VERSION 3.20)
project(MyProject)

# Controllo piattaforma
if(NOT TARGET_PLATFORM)
    message(FATAL_ERROR "Specificare TARGET_PLATFORM: -DTARGET_PLATFORM=PC o TEENSY41")
endif()

# Includi utilità
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(component)
include(kconfig)

# Seleziona defconfig e toolchain
if(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    set(CONFIG_BOARD_DEFCONFIG teensy41_defconfig)
    set(TOOLCHAIN_FILE toolchain_teensy41.cmake)
else()
    set(CONFIG_BOARD_DEFCONFIG pc_defconfig)
    set(TOOLCHAIN_FILE toolchain_native.cmake)
endif()

# Carica toolchain
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${TOOLCHAIN_FILE})

# Kconfig
set(DOT_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR}/.config)
set(DOT_CONFIG_BACKUP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/${CONFIG_BOARD_DEFCONFIG})

configure_kconfig(
    KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig
    DOT_CONFIG ${DOT_CONFIG_PATH}
    DOT_CONFIG_BACKUP ${DOT_CONFIG_BACKUP_PATH}
)

# Leggi .config in variabili CMake
if(EXISTS ${DOT_CONFIG_PATH})
    file(STRINGS ${DOT_CONFIG_PATH} CONFIG_CONTENTS)
    foreach(LINE ${CONFIG_CONTENTS})
        if(LINE MATCHES "^CONFIG_([a-zA-Z0-9_]+)=(.*)$")
            set(${CMAKE_MATCH_1} ${CMAKE_MATCH_2})
        elseif(LINE MATCHES "^# CONFIG_([a-zA-Z0-9_]+) is not set$")
            set(${CMAKE_MATCH_1} "n")
        endif()
    endforeach()
endif()

# Aggiungi componenti se abilitati
if(CONFIG_CORE_ENABLED)
    add_subdirectory(components/core)
endif()
if(CONFIG_SENSOR_ENABLED)
    add_subdirectory(components/sensor)
endif()
if(CONFIG_ACTUATOR_ENABLED)
    add_subdirectory(components/actuator)
endif()

# Aggiungi applicazione
#add_subdirectory(app)