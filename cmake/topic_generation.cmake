#-----------------------------------------------
#         Gestione generazione topic
#-----------------------------------------------

# Generazione automatica di param_defs.hpp / param_defs.cpp dagli YAML dei moduli
function(topic_generation)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    set(GENERATOR       ${CMAKE_SOURCE_DIR}/scripts/srimb/generate_topic.py)
    set(INPUT_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/topic)
    set(OUTPUT_DIR      ${CMAKE_CURRENT_BINARY_DIR}/generated)

    # Raccoglie tutti i .topic come dipendenze esplicite
    # CONFIGURE_DEPENDS aggiorna automaticamente la lista qunado si aggiungono/rimuovono file .topic
    file(GLOB TOPIC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/topic/*.topic")

    if(NOT TOPIC_FILES)
        message(WARNING "No .topic files found in ${CMAKE_CURRENT_SOURCE_DIR}/topic")
        return()
    endif()

    set(GENERATED_HEADERS)
    foreach(topic ${TOPIC_FILES})
        get_filename_component(name ${topic} NAME_WE)
        list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/data_types/${name}.h)
    endforeach()

    list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/data_types/DataTypes.h)
    list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/srimb_topics/TopicMetadata.h)
    list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/generated/srimb_topics/TopicTypes.h)

    add_custom_command(
        OUTPUT ${GENERATED_HEADERS}
        COMMAND python3 ${GENERATOR}
            --input-dir ${INPUT_DIR}
            --output-dir ${OUTPUT_DIR}
        DEPENDS ${TOPIC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/srimb/generate_topic.py
        COMMENT "Generating message headers from .topic files"
    )

    add_custom_target(generate_topic
        DEPENDS ${GENERATED_HEADERS}
    )
    
    set(TOPIC_GENERATED_HEADERS ${GENERATED_HEADERS} PARENT_SCOPE)

endfunction()