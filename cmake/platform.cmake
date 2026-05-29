#-----------------------------------------------
#         Gestione selezione piattaforma
#-----------------------------------------------


# Validazione e configurazione della piattaforma target
macro(configure_platform)
    if(NOT TARGET_PLATFORM)
        message(FATAL_ERROR
            "Specificare TARGET_PLATFORM:\n"
            "  -DTARGET_PLATFORM=SITL\n"
            "  -DTARGET_PLATFORM=TEENSY41"
        )
    endif()

    if(TARGET_PLATFORM STREQUAL "TEENSY41")
        set(DTC_OVERLAY_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/boards/infinity-fc/teensy41.overlay
            CACHE INTERNAL ""
        )
        set(CONF_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/prj.conf
            ${CMAKE_CURRENT_SOURCE_DIR}/boards/infinity-fc/teensy41.conf
            CACHE INTERNAL ""
        )
        message(STATUS "[Platform] Target: Teensy 4.1 (hardware)")

    elseif(TARGET_PLATFORM STREQUAL "SITL")
        set(DTC_OVERLAY_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/boards/sitl/native_sim.overlay
            CACHE INTERNAL ""
        )
        set(CONF_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/prj.conf
            ${CMAKE_CURRENT_SOURCE_DIR}/boards/sitl/native_sim.conf
            CACHE INTERNAL ""
        )
        message(STATUS "[Platform] Target: SITL (native_sim)")

    else()
        message(FATAL_ERROR "TARGET_PLATFORM non riconosciuto: ${TARGET_PLATFORM}")
    endif()
endmacro()