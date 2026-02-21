
add_library(entry STATIC)

if(${TARGET_PLATFORM} STREQUAL "SITL")
    target_sources(entry PRIVATE entry_sitl.cpp)
elseif(${TARGET_PLATFORM} STREQUAL "TEENSY41")
    target_sources(entry PRIVATE entry_teensy.cpp)

    add_custom_command(TARGET entry POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}_app ${PROJECT_NAME}_app.hex
        COMMENT "Generating HEX file for Teensy"
    )
endif()

target_include_directories(entry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)