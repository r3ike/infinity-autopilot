# cmake/toolchain_native.cmake
# Toolchain file per la compilazione nativa (host system)

# Il nome del sistema. CMake lo determinerà da solo, ma lo specifichiamo per chiarezza.
set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})

# Non impostare i compilatori esplicitamente lascia che CMake trovi quelli di default sul sistema.
# Questo è il comportamento voluto per una build nativa.
# Se volessi forzare l'uso di un compilatore specifico, potresti decommentare e modificare le righe seguenti:
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)
# set(CMAKE_ASM_COMPILER gcc)

# Flag di compilazione e linking (opzionali, puoi lasciarli gestire da CMake)
# set(CMAKE_C_FLAGS_INIT "")
# set(CMAKE_CXX_FLAGS_INIT "")
# set(CMAKE_EXE_LINKER_FLAGS_INIT "")

# Indica a CMake di cercare programmi, librerie e header nel sistema host.
# Queste impostazioni sono l'opposto di quelle usate per il cross-compiling.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)