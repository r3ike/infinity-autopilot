
add_subdirectory(SRIMB)
# add_subdirectory(params_manager)
add_subdirectory(scheduler)

add_library(infinity_platform INTERFACE)
target_include_directories(infinity_platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(infinity_platform INTERFACE zephyr_interface stdc++ lib)