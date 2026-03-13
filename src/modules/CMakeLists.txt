add_library(Modules INTERFACE)      #Target aggregatore

set(MODULES_LINK_LIB "")    #Lista di librerie da linkare a Modules

#---------------------------------
#       Moduli obbligatori
#---------------------------------

add_subdirectory(imu_manager)
list(APPEND MODULES_LINK_LIB modules_imu_manager)

#---------------------------------
#       Moduli Facoltativi
#---------------------------------

if(CONFIG_LOGGER_ENABLED)
    add_subdirectory(logger)
    list(APPEND MODULES_LINK_LIB modules_logger)
endif()


target_link_libraries(Modules INTERFACE ${MODULES_LINK_LIB})