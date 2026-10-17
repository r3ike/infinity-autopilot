
if(CONFIG_TARGET_SITL OR ${TARGET_PLATFORM} STREQUAL "SITL")
	add_library(posix_platform INTERFACE)

	target_include_directories(posix_platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(posix_platform INTERFACE zephyr_interface)
endif()
