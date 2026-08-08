add_library(scheduler INTERFACE)

target_include_directories(scheduler INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scheduler INTERFACE zephyr_interface stdc++ utils)