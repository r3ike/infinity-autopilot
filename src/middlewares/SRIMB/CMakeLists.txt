

# Specifica che questa è una libreria, che può essere condivisa (SHARED) o statica (STATIC)
add_library(SRIMB INTERFACE)

# Specifica dove sono gli header pubblici e rendi questa informazione parte del target
# PUBLIC: chiunque linki questa libreria erediterà questa directory di inclusione
target_include_directories(SRIMB INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Se la libreria avesse bisogno di altre librerie, le linkerebbe qui
target_link_libraries(SRIMB PUBLIC zephyr_interface stdc++ )
