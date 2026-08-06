cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(srimb_topic_test)

FILE(GLOB test_sources src/*.cpp)  # includi i sorgenti del modulo

target_link_libraries(app PRIVATE SRIMB)

target_sources(app PRIVATE ${test_sources})