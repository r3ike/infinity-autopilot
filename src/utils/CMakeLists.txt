add_library(utils INTERFACE)
#target_sources(utils PRIVATE Logger.cpp)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utils INTERFACE zephyr_interface stdc++)