# Interfaccia per i driver sitl

if(CONFIG_TARGET_SITL OR ${TARGET_PLATFORM} STREQUAL "SITL")
    add_subdirectory(radio)
    
    add_library(drivers_sitl INTERFACE)      #Target aggregatore
    
    set(DRIVERS_SITL_LINK_LIB "")    #Lista di librerie da linkare a Modules
    

    # Aggiungiamo le interfacce delle singole famiglie di driver all'interfaccia principale dei driver hardware
    if(CONFIG_RADIO_SITL_DRIVER_ENABLED)
        list(APPEND DRIVERS_SITL_LINK_LIB Radio_sitl_drivers)
    endif()
    //list(APPEND DRIVERS_SITL_LINK_LIB Gnss_sitl_drivers)


    target_include_directories(drivers_sitl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(drivers_sitl INTERFACE ${DRIVERS_SITL_LINK_LIB})
endif()


