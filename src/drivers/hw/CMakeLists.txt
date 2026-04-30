# Interfaccia per i driver hardware

if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(imu)
    add_subdirectory(gnss)
    
    add_library(drivers_hardware INTERFACE)      #Target aggregatore
    
    set(DRIVERS_HW_LINK_LIB "")    #Lista di librerie da linkare a Modules
    

    # Aggiungiamo le interfacce delle singole famiglie di driver all'interfaccia principale dei driver hardware
    list(APPEND DRIVERS_HW_LINK_LIB Imu_drivers)
    //list(APPEND DRIVERS_HW_LINK_LIB Gnss_drivers)


    target_include_directories(drivers_hardware INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(drivers_hardware INTERFACE ${DRIVERS_HW_LINK_LIB})
endif()


