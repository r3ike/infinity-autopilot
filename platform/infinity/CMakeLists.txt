
add_library(infinity_platform INTERFACE)      #Target aggregatore

set(LINK_LIB "")    #Lista di librerie da linkare a Modules

add_subdirectory(logging)
list(APPEND LINK_LIB logging)

add_subdirectory(scheduler)
list(APPEND LINK_LIB scheduler)

add_subdirectory(params_manager)
list(APPEND LINK_LIB params_manager)

add_subdirectory(SRIMB)
list(APPEND LINK_LIB SRIMB)

target_include_directories(infinity_platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(infinity_platform INTERFACE ${LINK_LIB})