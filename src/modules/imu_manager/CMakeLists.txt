
add_library(modules_imu_manager STATIC)
target_sources(modules_imu_manager PRIVATE 
                    imu_preprocessor/ImuPreprocessor.cpp
                    imu_preprocessor/acc_preprocessor/AccPreprocessor.cpp
                    imu_preprocessor/gyro_preprocessor/GyroPreprocessor.cpp
                    imu_preprocessor/merger/Merger.cpp
                )

target_include_directories(modules_imu_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(modules_imu_manager PUBLIC 
                            zephyr_interface 
                            SRIMB 
                            hal 
                            scheduler 
                            utils
                            stdc++
                        )