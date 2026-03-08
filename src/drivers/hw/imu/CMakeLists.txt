if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    if(CONFIG_BMI088_DRIVER_ENABLED)
        add_subdirectory(Bmi088_driver)
    endif()

    if(CONFIG_ICM42688P_DRIVER_ENABLED)
        add_subdirectory(ICM42688P_driver)
    endif()
endif()