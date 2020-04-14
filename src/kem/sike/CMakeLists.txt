if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    # The way various files are #include'd in the SIKE implementations leads to unused functions
    # We will not raise warnings for these
    add_compile_options(-Wno-unused-function)
    # GCC reports warnings about missing const related to how f2elm_t is typedef'd
    # Technically GCC is correct, but we will not raise warnings for these
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wno-pedantic)
        # On GCC 4.9 those warnings can't be turned off, so we disable -Werror
        if(CMAKE_C_COMPILER_VERSION VERSION_LESS "5")
            add_compile_options(-Wno-error)
        endif()
    endif()
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    # Clang emits spurious warnings where
    # the "{0}" structure initializer is used.
    add_compile_options(-Wno-missing-braces)
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
    # ignore warning about cast from int to OQS_STATUS
    add_compile_options(/wd4133)
endif()

set(SRCS kem_sike.c
         P434/P434.c
         P434/P434_compressed.c
         P503/P503.c P503/P503_compressed.c
         P610/P610.c
         P610/P610_compressed.c
         P751/P751.c
         P751/P751_compressed.c)

if(ARCH STREQUAL "x86_64")
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(SRCS ${SRCS} P503/AMD64/fp_x64_asm.S
                     P751/AMD64/fp_x64_asm.S)
    if(USE_BMI2_INSTRUCTIONS)
      set(SRCS ${SRCS} P434/AMD64/fp_x64_asm.S
                       P610/AMD64/fp_x64_asm.S)
      add_compile_options(-mbmi2)
    endif()
  endif()
elseif(ARCH STREQUAL "arm64")
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(SRCS ${SRCS} P503/AMD64/fp_x64_asm.S
                     P751/AMD64/fp_x64_asm.S)
  endif()
endif()

add_library(sike OBJECT ${SRCS})
if(ARCH STREQUAL "x86")
  target_compile_definitions(sike PRIVATE _GENERIC_ _X86_)
elseif(ARCH STREQUAL "x86_64")
  target_compile_definitions(sike PRIVATE _AMD64_)
elseif(ARCH STREQUAL "arm")
  target_compile_definitions(sike PRIVATE _GENERIC_ _ARM_)
elseif(ARCH STREQUAL "arm64")
  target_compile_definitions(sike PRIVATE _ARM64_)
endif()
