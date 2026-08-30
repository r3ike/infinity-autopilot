cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(platform)
configure_platform()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})


include(FetchContent)

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        master  # O un tag specifico come 3.4.0
)

FetchContent_MakeAvailable(eigen)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(Infinity-Autopilot VERSION 0.5 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Generazione codice parametri
include(params_generation)
param_generation()   # setta PARAM_DEFS_HPP e PARAM_DEFS_CPP

# ── Generazione codice parametri
include(topic_generation)
topic_generation()   # setta TOPIC_GENERATED_HEADERS

add_subdirectory(lib)

add_subdirectory(platform/infinity)

add_subdirectory(src/utils)
add_subdirectory(src/modules)
add_subdirectory(src/drivers)

# Aggiungi entry point
add_subdirectory(src/entry)

target_sources(app PRIVATE ${PARAM_DEFS_CPP})
zephyr_include_directories(${CMAKE_BINARY_DIR}/generated)
add_dependencies(app generate_params)
add_dependencies(app generate_topic)