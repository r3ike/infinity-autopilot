add_library(Modules INTERFACE)      #Target aggregatore

set(MODULES_LINK_LIB "")    #Lista di librerie da linkare a Modules

#---------------------------------
#       Moduli obbligatori
#---------------------------------
add_subdirectory(SRIMB)
list(APPEND MODULES_LINK_LIB SRIMB)
add_subdirectory(ImuManager)
list(APPEND MODULES_LINK_LIB Imu_manager)

#---------------------------------
#       Moduli Facoltativi
#---------------------------------

if(CONFIG_LOGGER_ENABLED)
    add_subdirectory(Logger)
    list(APPEND MODULES_LINK_LIB Logger)
endif()


target_link_libraries(Modules INTERFACE ${MODULES_LINK_LIB})