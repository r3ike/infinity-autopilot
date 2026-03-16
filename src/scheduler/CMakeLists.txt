add_library(scheduler STATIC)

target_sources(scheduler PRIVATE Scheduler.cpp)
target_include_directories(scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scheduler PUBLIC zephyr_interface stdc++ utils)