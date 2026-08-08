add_library(log_backend STATIC )
target_sources(log_backend PRIVATE log_backend_srimb.cpp
                                   log_backend.c
                                    )
                                    
target_include_directories(log_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(log_backend PUBLIC zephyr_interface utils stdc++ SRIMB)