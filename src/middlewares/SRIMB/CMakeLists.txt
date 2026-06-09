
cmake_minimum_required(VERSION 3.10)
project(SRIMB VERSION 1.0.0 LANGUAGES NONE)

# Specifica che questa è una libreria, che può essere condivisa (SHARED) o statica (STATIC)
add_library(SRIMB INTERFACE)

# Specifica dove sono gli header pubblici e rendi questa informazione parte del target
# PUBLIC: chiunque linki questa libreria erediterà questa directory di inclusione
target_include_directories(SRIMB INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Se la libreria avesse bisogno di altre librerie, le linkerebbe qui
# target_link_libraries(MyLib PUBLIC qualche_altra_libreria)

# Opzionale ma CONSIGLIATO: Genera un file di configurazione per permettere l'uso con find_package
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SRIMB DESTINATION include)
install(TARGETS SRIMB EXPORT SRIMBTargets)
install(EXPORT SRIMBTargets
    FILE SRIMBConfig.cmake
    NAMESPACE SRIMB::
    DESTINATION lib/cmake/SRIMB
)