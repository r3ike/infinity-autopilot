find_package(Eigen3 REQUIRED)

add_library(modules_ekf STATIC)
target_sources(modules_ekf PRIVATE ekf.cpp)

target_include_directories(modules_ekf PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/python
)

target_link_libraries(modules_ekf PUBLIC zephyr_interface SRIMB scheduler Eigen3::Eigen)