
add_library(Radio_sitl_drivers STATIC)
target_sources(Radio_sitl_drivers PRIVATE Radio_sitl.cpp)
target_include_directories(Radio_sitl_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Radio_sitl_drivers PUBLIC zephyr_interface hal utils srimb_topics stdc++)
