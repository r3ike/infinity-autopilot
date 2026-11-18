add_library(hal STATIC)

# Iterable section for the drivers self-registration
zephyr_iterable_section(NAME HalDriverRegistrar GROUP DATA_REGION ${XIP_ALIGN_WITH_INPUT})
