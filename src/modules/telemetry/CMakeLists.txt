add_library(modules_telemetry STATIC)

target_sources(modules_telemetry PRIVATE Scheduler.cpp)
target_include_directories(modules_telemetry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(modules_telemetry PUBLIC zephyr_interface SRIMB srimb_topics mavlink_library stdc++ utils)