add_library(hal STATIC)

# Iterable section for the drivers self-registration
zephyr_iterable_section(NAME HalDriverRegistrar GROUP DATA_REGION ${XIP_ALIGN_WITH_INPUT})


# --- Header ---
target_include_directories(hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- Sorgenti (test + moduli) ---
file(GLOB registry_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/registry/*.cpp
)

if(CONFIG_TARGET_HITL)
	add_subdirectory(adapters/hitl)
	target_sources(hal PRIVATE factory/HalFactory_hitl.cpp)
endif()

if(CONFIG_TARGET_SITL)
	add_subdirectory(adapters/sitl)
	target_sources(hal PRIVATE factory/HalFactory_sitl.cpp)
endif()

if(CONFIG_TARGET_TEENSY41)
	add_subdirectory(adapters/hw)
	target_sources(hal PRIVATE factory/HalFactory_hw.cpp)
endif()


target_sources(hal PRIVATE
    ${registry_sources}
)
