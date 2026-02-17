# cmake/kconfig.cmake
include_guard(GLOBAL)

find_package(Python3 REQUIRED)

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
    set(oneValueArgs KCONFIG_ROOT DOT_CONFIG DOT_CONFIG_BACKUP)
    set(multiValueArgs)

    cmake_parse_arguments(KCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(KCONFIG_ROOT ${KCONFIG_KCONFIG_ROOT})
    set(DOT_CONFIG ${KCONFIG_DOT_CONFIG})
    set(DOT_CONFIG_BACKUP ${KCONFIG_DOT_CONFIG_BACKUP})
    set(AUTOCONF_H ${CMAKE_CURRENT_BINARY_DIR}/include/generated/autoconf.h)

    # Crea la directory per autoconf.h
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/generated)

    # Comando per generare .config e autoconf.h usando genconfig.py
    add_custom_command(
        OUTPUT ${DOT_CONFIG} ${AUTOCONF_H}
        COMMAND ${Python3_EXECUTABLE} ${GENCONFIG_SCRIPT}
            --kconfig ${KCONFIG_ROOT}
            --dot-config ${DOT_CONFIG}
            --dot-config-backup ${DOT_CONFIG_BACKUP}
            --header-path ${AUTOCONF_H}
        DEPENDS ${KCONFIG_ROOT}
                ${DOT_CONFIG_BACKUP}
        COMMENT "Generating configuration from Kconfig"
        VERBATIM
    )

    # Target per lanciare l'interfaccia menuconfig
    add_custom_target(menuconfig
        COMMAND ${Python3_EXECUTABLE} ${MENUCONFIG_SCRIPT}
            --kconfig ${KCONFIG_ROOT}
            --dot-config ${DOT_CONFIG}
        DEPENDS ${KCONFIG_ROOT}
        COMMENT "Running menuconfig to edit configuration"
        VERBATIM
    )

    # Target che garantisce che la configurazione sia generata prima di compilare
    add_custom_target(generate_config DEPENDS ${DOT_CONFIG} ${AUTOCONF_H})

    # Aggiungi la directory di inclusione per autoconf.h
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
endfunction()