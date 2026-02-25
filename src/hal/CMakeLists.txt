add_library(hal STATIC HAL.cpp)

target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hal PUBLIC srimb_topics utils)

if (CONFIG_BN280_DRIVER_ENABLED)
    add_subdirectory(teensy/drivers/gps/Bn280_driver)
endif()