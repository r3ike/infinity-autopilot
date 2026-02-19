function(add_module name)
    add_library(${name} STATIC)
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()