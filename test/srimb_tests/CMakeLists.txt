cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(srimb_topic_test)

FILE(GLOB test_sources src/*.cpp)  # includi i sorgenti del modulo

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../platform/infinity/SRIMB srimb_build)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib lib)

target_link_libraries(app PRIVATE SRIMB zephyr_interface stdc++ lib)

target_sources(app PRIVATE ${test_sources})