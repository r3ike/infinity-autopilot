if(CONFIG_ICM42688P_DRIVER_ENABLED)
    add_library(driver_icm42699p STATIC)
    target_sources(driver_icm42699p PRIVATE ICM42688P_driver.cpp)
    target_include_directories(driver_icm42699p PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(driver_icm42699p PUBLIC zephyr_interface hal utils srimb_topics stdc++ Lpf_filters)
endif()