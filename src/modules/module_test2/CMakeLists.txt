
add_library(modules_test2 STATIC)
target_sources(modules_test2 PRIVATE Test2.cpp)
target_include_directories(modules_test2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(modules_test2 PUBLIC zephyr_interface SRIMB srimb_topics scheduler)