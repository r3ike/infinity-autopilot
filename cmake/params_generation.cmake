#-----------------------------------------------
#         Gestione generazione parametri
#-----------------------------------------------

# Generazione automatica di param_defs.hpp / param_defs.cpp dagli YAML dei moduli
function(param_generation)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    set(MODULES_DIR     ${CMAKE_SOURCE_DIR}/src/modules)
    set(GENERATOR       ${CMAKE_SOURCE_DIR}/scripts/params/generate_params.py)
    set(OUT_HPP         ${CMAKE_BINARY_DIR}/generated/param_defs.hpp)
    set(OUT_CPP         ${CMAKE_BINARY_DIR}/generated/param_defs.cpp)

    # Raccoglie tutti i module.yaml come dipendenze esplicite
    file(GLOB_RECURSE YAML_FILES "${MODULES_DIR}/*.yaml")

    add_custom_command(
        OUTPUT  ${OUT_HPP} ${OUT_CPP}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated
        COMMAND ${Python3_EXECUTABLE} ${GENERATOR} ${MODULES_DIR} ${CMAKE_BINARY_DIR}/generated
        DEPENDS ${GENERATOR} ${YAML_FILES}
        COMMENT "[ParamsGen] Generazione param_defs da module.yaml"
        VERBATIM
    )

    add_custom_target(generate_params
        DEPENDS ${OUT_HPP} ${OUT_CPP}
    )
    
    # Espone le variabili al chiamante tramite PARENT_SCOPE
    set(PARAM_DEFS_HPP ${OUT_HPP} PARENT_SCOPE)
    set(PARAM_DEFS_CPP ${OUT_CPP} PARENT_SCOPE)

endfunction()