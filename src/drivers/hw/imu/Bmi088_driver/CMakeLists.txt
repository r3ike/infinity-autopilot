if(CONFIG_BMI088_DRIVER_ENABLED)
    add_library(driver_bmi088 STATIC)
    target_sources(driver_bmi088 PRIVATE Bmi088_driver.cpp)
    target_include_directories(driver_bmi088 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(driver_bmi088 PUBLIC zephyr_interface hal utils srimb_topics stdc++ Lpf_filters)
endif()