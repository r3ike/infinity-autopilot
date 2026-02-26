add_library(hal STATIC HAL.cpp)

target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hal PUBLIC srimb_topics utils bmi088_driver bn280_driver)

if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(teensy/cores/teensy4)

    if (CONFIG_BN280_DRIVER_ENABLED)
        add_subdirectory(teensy/drivers/gps/Bn280_driver)
    endif()

    if(CONFIG_BMI088_DRIVER_ENABLED)
        add_subdirectory(teensy/drivers/imu/Bmi088_driver)
    endif()

else()
    message(FATAL_ERROR "Target non supportato: ${CONFIG_TARGET}")

endif()