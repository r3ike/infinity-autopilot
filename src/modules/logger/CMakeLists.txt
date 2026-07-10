if(CONFIG_LOGGER_ENABLED)
    add_library(modules_logger STATIC)
    target_sources(modules_logger PRIVATE Logger.cpp)

    target_include_directories(modules_logger 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(modules_logger 
        PRIVATE zephyr_interface 
        PUBLIC  SRIMB
                hal
                scheduler
                utils
                stdc++
                )
    
endif()
