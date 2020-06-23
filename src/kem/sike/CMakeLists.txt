# SPDX-License-Identifier: MIT

if(OQS_ENABLE_KEM_sike_p434 OR
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
   OQS_ENABLE_KEM_sidh_p751_compressed)

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

    if(ARCH STREQUAL "x86_64")
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                set(SRCS ${SRCS} external/P503/AMD64/fp_x64_asm.S)
	    endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                set(SRCS ${SRCS} external/P751/AMD64/fp_x64_asm.S)
	    endif()
            if(OQS_USE_BMI2_INSTRUCTIONS AND NOT OQS_PORTABLE_BUILD)
                if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                    set(SRCS ${SRCS} external/P434/AMD64/fp_x64_asm.S)
                endif()
                if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                    set(SRCS ${SRCS} external/P610/AMD64/fp_x64_asm.S)
	        endif()
                add_compile_options(-mbmi2)
            endif()
        endif()
    elseif(ARCH STREQUAL "arm64")
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
    elseif(ARCH STREQUAL "x86_64")
        target_compile_definitions(sike PRIVATE _AMD64_)
        if(OQS_USE_BMI2_INSTRUCTIONS AND NOT OQS_PORTABLE_BUILD)
            target_compile_definitions(sike PRIVATE _MULX_ _ADX_)
        endif()
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            if(OQS_ENABLE_KEM_sike_p503 OR OQS_ENABLE_KEM_sidh_p503 OR OQS_ENABLE_KEM_sike_p503_compressed OR OQS_ENABLE_KEM_sidh_p503_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP503_ASM)
            endif()
            if(OQS_ENABLE_KEM_sike_p751 OR OQS_ENABLE_KEM_sidh_p751 OR OQS_ENABLE_KEM_sike_p751_compressed OR OQS_ENABLE_KEM_sidh_p751_compressed)
                target_compile_definitions(sike PRIVATE USE_SIKEP751_ASM)
            endif()
            if(OQS_USE_BMI2_INSTRUCTIONS AND NOT OQS_PORTABLE_BUILD)
                if(OQS_ENABLE_KEM_sike_p434 OR OQS_ENABLE_KEM_sidh_p434 OR OQS_ENABLE_KEM_sike_p434_compressed OR OQS_ENABLE_KEM_sidh_p434_compressed)
                    target_compile_definitions(sike PRIVATE USE_SIKEP434_ASM)
		endif()
                if(OQS_ENABLE_KEM_sike_p610 OR OQS_ENABLE_KEM_sidh_p610 OR OQS_ENABLE_KEM_sike_p610_compressed OR OQS_ENABLE_KEM_sidh_p610_compressed)
                    target_compile_definitions(sike PRIVATE USE_SIKEP610_ASM)
                endif()
            endif()
        endif()
    elseif(ARCH STREQUAL "arm")
        target_compile_definitions(sike PRIVATE _GENERIC_ _ARM_)
    elseif(ARCH STREQUAL "arm64")
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
    endif()
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_definitions(sike PRIVATE COMPILER_GCC)
    elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_definitions(sike PRIVATE COMPILER_CLANG)
    endif()
endif()
