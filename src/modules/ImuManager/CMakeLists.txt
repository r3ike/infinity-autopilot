
add_module(logger)
target_sources(logger PRIVATE ImuManager.cpp)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(logger PUBLIC zephyr_interface SRIMB srimb_topics srimb hal)

