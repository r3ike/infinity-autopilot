add_library(hal STATIC HAL.cpp)

target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hal PUBLIC zephyr_interface srimb_topics utils bmi088_driver)

if(CONFIG_TARGET_TEENSY41 OR ${TARGET_PLATFORM} STREQUAL "TEENSY41")
    add_subdirectory(hw)
else()
    add_subdirectory(sitl)
endif()