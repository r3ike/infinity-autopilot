cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(srimb_tests)

# --- Header ---
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/infinity/SRIMB
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/infinity/scheduler
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

# --- Sorgenti (test + moduli) ---
file(GLOB test_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB srimb_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/infinity/SRIMB/*.cpp
)
file(GLOB sched_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/infinity/scheduler/*.cpp
)
file(GLOB lib_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/*.cpp
)

target_sources(app PRIVATE
    ${test_sources}
    ${srimb_sources}
    ${sched_sources}
    ${lib_sources}
)
