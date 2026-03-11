
add_library(Imu_manager STATIC)
target_sources(Imu_manager PRIVATE ImuManager.cpp)
target_include_directories(Imu_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Imu_manager PUBLIC zephyr_interface SRIMB srimb_topics hal)
