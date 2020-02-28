if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    # The way various files are #include'd in the SIKE implementations leads to unused functions
    # We will not raise warnings for these
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function ")
    # GCC reports warnings about missing const related to how f2elm_t is typedef'd
    # Technically GCC is correct, but we will not raise warnings for these
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pedantic ")
        # On GCC 4.9 those warnings can't be turned off, so we disable -Werror
        if(CMAKE_C_COMPILER_VERSION VERSION_LESS "5")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error ")
        endif()
    endif()
endif()

set(SRCS kem_sike.c
         P434/P434.c
         P434/P434_compressed.c
         P503/P503.c P503/P503_compressed.c
         P610/P610.c
         P610/P610_compressed.c
         P751/P751.c
         P751/P751_compressed.c)

add_library(sike OBJECT ${SRCS})

# FIXMEOQS: enable FAST mode, assembly
target_compile_definitions(sike PRIVATE _GENERIC_)
if(ARCH STREQUAL "x86")
    target_compile_definitions(sike PRIVATE _X86_)
elseif(ARCH STREQUAL "x86_64")
    target_compile_definitions(sike PRIVATE _AMD64_)
elseif(ARCH STREQUAL "arm")
    target_compile_definitions(sike PRIVATE _ARM_)
elseif(ARCH STREQUAL "arm64")
    target_compile_definitions(sike PRIVATE _ARM64_)
endif()
