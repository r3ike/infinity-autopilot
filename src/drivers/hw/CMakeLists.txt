if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(imu)
    add_subdirectory(gnss)
endif()