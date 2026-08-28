add_library(logging STATIC )
target_sources(logging PRIVATE Loggable.cpp)
target_include_directories(logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(logging PUBLIC zephyr_interface stdc++)