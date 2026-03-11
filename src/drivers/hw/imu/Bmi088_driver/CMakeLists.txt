if(CONFIG_BMI088_DRIVER_ENABLED)
    add_library(Bmi088_driver STATIC)
    target_sources(Bmi088_driver PRIVATE Bmi088_driver.cpp)
    target_include_directories(Bmi088 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Bmi088_driver PUBLIC zephyr_interface hal utils srimb_topics stdc++ Lpf_filters)
endif()