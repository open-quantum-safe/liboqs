# SPDX-License-Identifier: MIT

if(
  # check we are on a supported platform (SIKE's config.h stops the build otherwise)
  (ARCH STREQUAL "x86" OR
   ARCH_X86 OR
   ARCH_X86_64 OR
   ARCH_ARM32v7 OR
   ARCH_ARM64v8 OR
   ARCH_PPC64LE OR
   ARCH_S390X)
  AND
  # check that one SIDH/SIKE alg is enabled
  (OQS_ENABLE_KEM_sike_p434 OR
   OQS_ENABLE_KEM_sike_p503 OR
   OQS_ENABLE_KEM_sike_p610 OR
   OQS_ENABLE_KEM_sike_p751 OR
   OQS_ENABLE_KEM_sike_p434_compressed OR
   OQS_ENABLE_KEM_sike_p503_compressed OR
   OQS_ENABLE_KEM_sike_p610_compressed OR
   OQS_ENABLE_KEM_sike_p751_compressed OR
   OQS_ENABLE_KEM_sidh_p434 OR
   OQS_ENABLE_KEM_sidh_p503 OR
   OQS_ENABLE_KEM_sidh_p610 OR
   OQS_ENABLE_KEM_sidh_p751 OR
   OQS_ENABLE_KEM_sidh_p434_compressed OR
   OQS_ENABLE_KEM_sidh_p503_compressed OR
   OQS_ENABLE_KEM_sidh_p610_compressed OR
   OQS_ENABLE_KEM_sidh_p751_compressed))

    set(SRCS kem_sike.c)
    if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434)
        set(SRCS ${SRCS} external/P434/P434.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
        set(SRCS ${SRCS} external/P434/P434_compressed.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503)
        set(SRCS ${SRCS} external/P503/P503.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
        set(SRCS ${SRCS} external/P503/P503_compressed.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610)
        set(SRCS ${SRCS} external/P610/P610.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
        set(SRCS ${SRCS} external/P610/P610_compressed.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751)
        set(SRCS ${SRCS} external/P751/P751.c)
    endif()
    if(OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
        set(SRCS ${SRCS} external/P751/P751_compressed.c)
    endif()

    if(ARCH_X86_64)
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                set(SRCS ${SRCS} external/P503/AMD64/fp_x64_asm.S)
            endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                set(SRCS ${SRCS} external/P751/AMD64/fp_x64_asm.S)
            endif()
            if(OQS_USE_BMI2_INSTRUCTIONS)
                # The p434 and p610 fp_x64_asm.S files support x64+BMI2 and x64+BMI2+ADX, but not x64
                if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                    set(SRCS ${SRCS} external/P434/AMD64/fp_x64_asm.S)
                endif()
                if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                    set(SRCS ${SRCS} external/P610/AMD64/fp_x64_asm.S)
                endif()
            endif()
        endif()
    elseif(ARCH_ARM64v8)
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                set(SRCS ${SRCS} external/P434/ARM64/fp_arm64_asm.S)
            endif()
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                set(SRCS ${SRCS} external/P503/ARM64/fp_arm64_asm.S)
            endif()
            if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                set(SRCS ${SRCS} external/P610/ARM64/fp_arm64_asm.S)
            endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                set(SRCS ${SRCS} external/P751/ARM64/fp_arm64_asm.S)
            endif()
        endif()
    endif()

    add_library(sike OBJECT ${SRCS})
    if(ARCH STREQUAL "x86")
        target_compile_definitions(sike PRIVATE _GENERIC_ _X86_)
    elseif(ARCH_X86)
        target_compile_definitions(sike PRIVATE _GENERIC_ _X86_)
    elseif(ARCH_X86_64)
        target_compile_definitions(sike PRIVATE _AMD64_)
        if(OQS_USE_BMI2_INSTRUCTIONS)
            target_compile_definitions(sike PRIVATE _MULX_)
            if(OQS_USE_ADX_INSTRUCTIONS)
                target_compile_definitions(sike PRIVATE _ADX_)
            endif()
        endif()
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP503_ASM)
            endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP751_ASM)
            endif()
            if(OQS_USE_BMI2_INSTRUCTIONS)
                # The p434 and p610 fp_x64_asm.S files support x64+BMI2 and x64+BMI2+ADX, but not x64
                if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                    target_compile_definitions(sike PRIVATE USE_SIKEP434_ASM)
                endif()
                if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                    target_compile_definitions(sike PRIVATE USE_SIKEP610_ASM)
                endif()
            endif()
        endif()
    elseif(ARCH_ARM32v7)
        target_compile_definitions(sike PRIVATE _GENERIC_ _ARM_)
    elseif(ARCH_ARM64v8)
        target_compile_definitions(sike PRIVATE _ARM64_)
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP434_ASM)
            endif()
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP503_ASM)
            endif()
            if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP610_ASM)
            endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP751_ASM)
            endif()
        endif()
    elseif(ARCH_PPC64LE)
        target_compile_definitions(sike PRIVATE _GENERIC_ _PPC64LE_)
    elseif(ARCH_S390X)
        target_compile_definitions(sike PRIVATE _GENERIC_ _S390X_)
    endif()

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_definitions(sike PRIVATE COMPILER_GCC)
    elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_definitions(sike PRIVATE COMPILER_CLANG)
    endif()

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
        # The way various files are #include'd in the SIKE implementations leads to unused functions
        # We will not raise warnings for these
        target_compile_options(sike PRIVATE -Wno-unused-function)
        # GCC reports warnings about missing const related to how f2elm_t is typedef'd
        # Technically GCC is correct, but we will not raise warnings for these
        if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
            target_compile_options(sike PRIVATE -Wno-pedantic)
            # On GCC 4.9 those warnings can't be turned off, so we disable -Werror
            if(CMAKE_C_COMPILER_VERSION VERSION_LESS "5")
                target_compile_options(sike PRIVATE -Wno-error)
            endif()
        endif()
    endif()
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        # Clang emits spurious warnings where
        # the "{0}" structure initializer is used.
        target_compile_options(sike PRIVATE -Wno-missing-braces)
    endif()
    if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
        # ignore warning about cast from int to OQS_STATUS
        target_compile_options(sike PRIVATE /wd4133)
    endif()
endif()
