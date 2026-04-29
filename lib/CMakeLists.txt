add_library(lib INTERFACE)

target_include_directories(lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lib INTERFACE stdc++)