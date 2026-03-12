add_library(Imu_drivers INTERFACE)      #Target aggregatore

set(IMU_DRIVERS_LINK_LIB "")    #Lista di librerie da linkare a Imu_drivers

if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    if(CONFIG_BMI088_DRIVER_ENABLED)
        add_subdirectory(Bmi088_driver)

        list(APPEND IMU_DRIVERS_LINK_LIB driver_bmi088)
    endif()

    if(CONFIG_ICM42688P_DRIVER_ENABLED)
        add_subdirectory(ICM42688P_driver)

        list(APPEND IMU_DRIVERS_LINK_LIB driver_icm42699p)
    endif()
endif()

target_link_libraries(Imu_drivers INTERFACE ${IMU_DRIVERS_LINK_LIB})