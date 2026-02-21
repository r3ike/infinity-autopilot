cmake_minimum_required(VERSION 3.20)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(Infinity-Autopilot VERSION 0.5 LANGUAGES C CXX)

#add_executable(Infinity-Autopilot)

#target_link_libraries(Infinity-Autopilot PUBLIC entry)

# Controllo piattaforma
if(NOT TARGET_PLATFORM)
    message(FATAL_ERROR "Specificare TARGET_PLATFORM: -DTARGET_PLATFORM=SITL o TEENSY41")
endif()

# Includi utilità
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(module)
include(kconfig)

# Seleziona defconfig e toolchain
if(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    set(CONFIG_BOARD_DEFCONFIG teensy41_defconfig)
    set(TOOLCHAIN_FILE toolchain_teensy41.cmake)
else()
    set(CONFIG_BOARD_DEFCONFIG pc_defconfig)
    set(TOOLCHAIN_FILE toolchain_sitl.cmake)
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
if(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(src/hal/teensy/lib/TinyGPSPlus)
endif()

# Aggiungi componenti se abilitati
if(CONFIG_LOGGER_ENABLED)
    add_subdirectory(src/modules/Logger)
endif()


if(BN280_DRIVER_ENABLED AND ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(src/hal/teensy/drivers/gps/Bn280_driver)
endif()


# Aggiungi applicazione
add_subdirectory(src/entry)