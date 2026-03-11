if(CONFIG_ICM42688P_DRIVER_ENABLED)
    add_library(Icm42688p_driver STATIC)
    target_sources(Icm42688p_driver PRIVATE ICM42688P_driver.cpp)
    target_include_directories(Icm42688p_driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Icm42688p_driver PUBLIC zephyr_interface hal utils srimb_topics stdc++ Lpf_filters)
endif()