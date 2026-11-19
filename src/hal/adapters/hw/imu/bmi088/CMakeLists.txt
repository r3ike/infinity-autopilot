if(CONFIG_BMI088_DRIVER_ENABLED)
	add_library(bmi088_adapter STATIC)

	file(GLOB bmi088_sources
	    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	)
	target_sources(bmi088_adapter PRIVATE ${bmi088_sources})

	target_include_directories(bmi088_adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

	target_link_libraries(bmi088_adapter PRIVATE zephyr_interface 
											   	  hal
											   	  utils
											   	  stdc++
											   	  infinity_platform
											   )
	add_dependencies(bmi088_adapter offsets_h)
endif()
