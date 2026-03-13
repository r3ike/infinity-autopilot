# Crea l'eseguibile principale
#target_sources(app PUBLIC)



target_sources(app PRIVATE entry.cpp FlightController.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC zephyr_interface stdc++ utils hal Modules scheduler)
