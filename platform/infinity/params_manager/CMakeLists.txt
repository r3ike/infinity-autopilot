add_library(params_manager STATIC )
target_sources(params_manager PRIVATE param_manager.cpp)
target_include_directories(params_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(params_manager PUBLIC zephyr_interface utils stdc++)