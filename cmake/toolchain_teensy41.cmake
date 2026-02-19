# cmake/toolchain_teensy41.cmake
# Nome del sistema (importante per CMake)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

# Specifica il compilatore cross-compilatore
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR           arm-none-eabi-ar)
set(CMAKE_OBJCOPY      arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP      arm-none-eabi-objdump)
set(CMAKE_SIZE         arm-none-eabi-size)

# Trova la toolchain (assumendo che sia nel PATH)
find_program(CMAKE_C_COMPILER arm-none-eabi-gcc)
if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "arm-none-eabi-gcc not found. Please install the ARM toolchain.")
endif()

# Imposta i flag di compilazione per Teensy 4.1 (Cortex-M7 con FPU)
set(MCU_SPECIFIC_FLAGS "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb")

set(COMMON_FLAGS "${MCU_SPECIFIC_FLAGS} -ffunction-sections -fdata-sections -Wall -g -Os")
set(CMAKE_C_FLAGS_INIT   "${COMMON_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=gnu++17 -fno-exceptions -fno-rtti")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-Wl,--gc-sections ${MCU_SPECIFIC_FLAGS} -T${CMAKE_SOURCE_DIR}/src/hal/teensy/cores/teensy4/imxrt1062_t41.ld"
)

# Non cercare librerie nel sistema host
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)