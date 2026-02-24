# Crea l'eseguibile principale
add_executable(infinity_autopilot)

if(${TARGET_PLATFORM} STREQUAL "SITL")
    target_sources(infinity_autopilot PRIVATE entry_sitl.cpp)
elseif(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    target_sources(infinity_autopilot PRIVATE entry_teensy.cpp)
    target_link_libraries(infinity_autopilot PUBLIC arduino_core)

    # Genera il file HEX per Teensy dopo la build
    add_custom_command(TARGET infinity_autopilot POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:infinity_autopilot> ${PROJECT_NAME}.hex
        COMMENT "Generating HEX file for Teensy"
    )
endif()

target_include_directories(infinity_autopilot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)