cmake_minimum_required(VERSION 3.10)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(Infinity-Autopilot VERSION 0.5 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
    set(CONFIG_BOARD_DEFCONFIG sitl_defconfig)
    set(TOOLCHAIN_FILE toolchain_sitl.cmake)
endif()

# Carica toolchain
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${TOOLCHAIN_FILE})

# Kconfig
set(DOT_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.config)
set(DEF_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/${CONFIG_BOARD_DEFCONFIG})

configure_kconfig(
    KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig
    DOT_CONFIG ${DOT_CONFIG_PATH}
    DEFCONFIG_FILE ${DEF_CONFIG_PATH}
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


add_subdirectory(src/utils)
add_subdirectory(src/modules/SRIMB)
add_subdirectory(src/utils/srimb_topics)
add_subdirectory(src/hal)

# Aggiungi componenti se abilitati
if(CONFIG_LOGGER_ENABLED)
    add_subdirectory(src/modules/Logger)
endif()



# Aggiungi entry point
add_subdirectory(src/entry)

add_dependencies(infinity_autopilot generate_config)        #assicura che autoconf.h sia generato prima della compilazione