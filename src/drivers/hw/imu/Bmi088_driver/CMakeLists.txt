if(CONFIG_BMI088_DRIVER_ENABLED)
    add_module(bmi088_driver)
    target_sources(bmi088_driver PRIVATE Bmi088_driver.cpp)

    target_link_libraries(bmi088_driver PUBLIC hal utils srimb_topics)
endif()