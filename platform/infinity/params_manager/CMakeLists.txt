add_library(params_magager STATIC )
target_sources(params_magager PRIVATE param_magager.cpp)
target_include_directories(params_magager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(params_magager PUBLIC zephyr_interface utils stdc++)