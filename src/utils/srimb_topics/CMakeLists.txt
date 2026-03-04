
set(TOPICS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/baro_topic/baro_topic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imu_topic/imu_topic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gps_topic/gps_topic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lidar_topic/lidar_topic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mag_topic/mag_topic.cpp
)

add_library(srimb_topics STATIC ${TOPICS_SOURCES})
target_include_directories(srimb_topics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(srimb_topics PUBLIC SRIMB utils)