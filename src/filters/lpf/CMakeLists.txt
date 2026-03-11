add_library(Lpf_filters INTERFACE)
target_include_directories(Lpf_filters INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Lpf_filters INTERFACE zephyr_interface srimb_topics stdc++)