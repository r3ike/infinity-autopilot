# cmake/kconfig.cmake
include_guard(GLOBAL)

find_package(Python3 REQUIRED)
#message(STATUS "Python used: ${Python3_EXECUTABLE}")
# Trova il percorso della directory in cui si trova il modulo kconfiglib
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import kconfiglib; import os; print(os.path.dirname(kconfiglib.__file__))"
    OUTPUT_VARIABLE KCONFIGLIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Percorsi degli script forniti da kconfiglib
set(GENCONFIG_SCRIPT ${KCONFIGLIB_DIR}/genconfig.py)
set(MENUCONFIG_SCRIPT ${KCONFIGLIB_DIR}/menuconfig.py)

function(configure_kconfig)
    set(options)
    set(oneValueArgs KCONFIG_ROOT DOT_CONFIG DEFCONFIG_FILE)
    set(multiValueArgs)

    cmake_parse_arguments(KCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(KCONFIG_ROOT ${KCONFIG_KCONFIG_ROOT})
    set(DOT_CONFIG ${KCONFIG_DOT_CONFIG})
    set(DEFCONFIG_FILE ${KCONFIG_DEFCONFIG_FILE})
    set(AUTOCONF_H ${CMAKE_CURRENT_BINARY_DIR}/include/generated/autoconf.h)

    # Crea la directory per autoconf.h
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/generated)

    # Se non esiste .config e abbiamo un defconfig, copialo
    if(DEFINED DEFCONFIG_FILE AND NOT EXISTS ${DOT_CONFIG})
        add_custom_command(
            OUTPUT ${DOT_CONFIG}
            COMMAND ${CMAKE_COMMAND} -E copy ${DEFCONFIG_FILE} ${DOT_CONFIG}
            COMMENT "Copying default configuration to ${DOT_CONFIG}"
            VERBATIM
        )
    endif()

    # Comando per generare autoconf.h da .config
    add_custom_command(
        OUTPUT ${AUTOCONF_H}
        COMMAND ${CMAKE_COMMAND} -E env 
            KCONFIG_CONFIG=${DOT_CONFIG}
            srctree=${CMAKE_SOURCE_DIR}
            ${Python3_EXECUTABLE} ${GENCONFIG_SCRIPT}
            --header-path ${AUTOCONF_H}
            ${KCONFIG_ROOT}
        DEPENDS ${KCONFIG_ROOT} ${DOT_CONFIG}
        COMMENT "Generating autoconf.h from .config"
        VERBATIM
    )

    # Target per menuconfig (modifica interattiva di .config)
    add_custom_target(menuconfig
        COMMAND ${CMAKE_COMMAND} -E env
            KCONFIG_CONFIG=${DOT_CONFIG}
            srctree=${CMAKE_SOURCE_DIR}
            ${Python3_EXECUTABLE} ${MENUCONFIG_SCRIPT}
            ${KCONFIG_ROOT}
        DEPENDS ${KCONFIG_ROOT}
        COMMENT "Running menuconfig to edit configuration"
        VERBATIM
    )

    # Target che garantisce la generazione di autoconf.h prima della compilazione
    add_custom_target(generate_config DEPENDS ${AUTOCONF_H})

    # Aggiungi la directory di inclusione per autoconf.h
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
endfunction()