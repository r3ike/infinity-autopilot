#-----------------------------------------------
#         Gestione generazione topic
#-----------------------------------------------

# Generazione automatica di param_defs.hpp / param_defs.cpp dagli YAML dei moduli
function(topic_generation)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    set(GENERATOR       ${CMAKE_SOURCE_DIR}/script/srimb/generate_topic.py)
    set(INPUT_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/topic)
    set(OUTPUT_DIR      ${CMAKE_CURRENT_BINARY_DIR}/generated/srimb_topic)
    set(OUT_HPP         ${CMAKE_BINARY_DIR}/generated/param_defs.hpp)
    set(OUT_CPP         ${CMAKE_BINARY_DIR}/generated/param_defs.cpp)

    set(GENERATED_HEADERS)
    foreach(msg ${MSG_FILES})
        get_filename_component(name ${msg} NAME_WE)
        list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/srimb_topic/${name}.h)
    endforeach()


    # Raccoglie tutti i .topic come dipendenze esplicite
    file(GLOB TOPIC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/topic/*.topic")

    add_custom_command(
        OUTPUT ${GENERATED_HEADERS}
        COMMAND python3 ${GENERATOR}
            --input-dir ${INPUT_DIR}
            --output-dir ${OUTPUT_DIR}
        DEPENDS ${MSG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_topic.py
        COMMENT "Generating message headers from .topic files"
    )

    add_custom_target(generate_params
        DEPENDS ${OUT_HPP} ${OUT_CPP}
    )
    
    # Espone le variabili al chiamante tramite PARENT_SCOPE
    set(PARAM_DEFS_HPP ${OUT_HPP} PARENT_SCOPE)
    set(PARAM_DEFS_CPP ${OUT_CPP} PARENT_SCOPE)

endfunction()