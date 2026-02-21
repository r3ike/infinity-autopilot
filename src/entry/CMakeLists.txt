# Crea l'eseguibile principale
add_executable(Infinity-Autopilot)

if(${TARGET_PLATFORM} STREQUAL "SITL")
    target_sources(Infinity-Autopilot PRIVATE entry_sitl.cpp)
elseif(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    target_sources(Infinity-Autopilot PRIVATE entry_teensy.cpp)
    target_link_libraries(Infinity-Autopilot PUBLIC arduino_core)

    # Genera il file HEX per Teensy dopo la build
    add_custom_command(TARGET Infinity-Autopilot POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:Infinity-Autopilot> ${PROJECT_NAME}.hex
        COMMENT "Generating HEX file for Teensy"
    )
endif()

target_include_directories(Infinity-Autopilot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)