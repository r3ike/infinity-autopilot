if(CONFIG_BN280_DRIVER_ENABLED)
    add_module(bn280_driver)
    target_sources(bn280_driver PRIVATE Bn280_driver.cpp)
endif()