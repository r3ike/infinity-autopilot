# Crea l'eseguibile principale
target_sources(app PUBLIC)

if(${TARGET_PLATFORM} STREQUAL "SITL")
    target_sources(app PRIVATE entry_sitl.cpp)
elseif(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    target_sources(app PRIVATE entry.cpp)

    # Genera il file HEX per Teensy dopo la build
    # add_custom_command(TARGET infinity_autopilot POST_BUILD
    #    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:infinity_autopilot> ${PROJECT_NAME}.hex
    #    COMMENT "Generating HEX file for Teensy"
    #)
endif()

