add_library(Imu_adapters INTERFACE)      #Target aggregatore

set(IMU_DRIVERS_LINK_LIB "")    #Lista di librerie da linkare a Imu_drivers

if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    if(CONFIG_BMI088_DRIVER_ENABLED)
        add_subdirectory(bmi088)

        list(APPEND IMU_DRIVERS_LINK_LIB bmi088_adapter)
    endif()

    if(CONFIG_ICM42688P_DRIVER_ENABLED)
        add_subdirectory(icm42688p)

        list(APPEND IMU_DRIVERS_LINK_LIB driver_icm42699p)
    endif()
endif()

target_link_libraries(Imu_adapters INTERFACE ${IMU_DRIVERS_LINK_LIB})
