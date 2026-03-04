# Crea una libreria statica chiamata "tinygps"
add_library(tinygps STATIC TinyGPS++.cpp)

# Rendi pubblica la directory corrente per l'header TinyGPS++.h
target_include_directories(tinygps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})