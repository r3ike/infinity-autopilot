if(CONFIG_BMI088_DRIVER_ENABLED)
    add_library(driver_bmi088 INTERFACE)
    target_include_directories(driver_bmi088 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(driver_bmi088 INTERFACE zephyr_interface hal utils stdc++ SRIMB scheduler)
    add_dependencies(driver_bmi088 offsets_h)
endif()