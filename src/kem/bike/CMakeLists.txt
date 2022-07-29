# SPDX-License-Identifier: MIT

set(OBJS "")
set(SOURCES "")

if(OQS_ENABLE_KEM_bike_l1 OR OQS_ENABLE_KEM_bike_l3)
    set(OQS_KEM_BIKE_CPP_DEFS_R3 "")

    list(APPEND SOURCES kem_bike.c)

    add_library(kem_bike OBJECT)
    oqs_target_init(kem_bike)

    oqs_add_c_flags(kem_bike REQUIRED PRIVATE ${OQS_C_FLAGS})
    oqs_add_ld_flags(kem_bike PRIVATE ${OQS_LD_FLAGS})
    target_include_directories(kem_bike PRIVATE ${OQS_INCLUDE_DIRS})

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND
       CMAKE_C_COMPILER_VERSION VERSION_LESS "5" OR
       CMAKE_C_COMPILER_ID MATCHES "Clang")
      # GCC 4.9 erroneously reports warnings for certain valid code in BIKE
      # https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer
      oqs_add_c_flags(kem_bike REQUIRED PRIVATE "-Wno-missing-braces" "-Wno-missing-field-initializers")
    endif()

    set_target_properties(kem_bike PROPERTIES SOURCES "${SOURCES}")

    set(OBJS $<TARGET_OBJECTS:kem_bike>)

    set(SRCS_R3 additional_r3/aes_ctr_prf.c
                additional_r3/decode.c
                additional_r3/decode_portable.c
                additional_r3/error.c
                additional_r3/gf2x_inv.c
                additional_r3/gf2x_ksqr_portable.c
                additional_r3/gf2x_mul.c
                additional_r3/gf2x_mul_base_portable.c
                additional_r3/gf2x_mul_portable.c
                additional_r3/kem.c
                additional_r3/sampling.c
                additional_r3/sampling_portable.c
                additional_r3/utilities.c)

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_AVX2_INSTRUCTIONS)
        set(AVX2_SRCS additional_r3/decode_avx2.c
                      additional_r3/gf2x_ksqr_avx2.c
                      additional_r3/gf2x_mul_avx2.c
                      additional_r3/sampling_avx2.c)
        set_source_files_properties(${AVX2_SRCS} PROPERTIES COMPILE_FLAGS "-mavx2")
        list(APPEND SRCS_R3 ${AVX2_SRCS})
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_AVX512_INSTRUCTIONS)
        set(AVX512_SRCS additional_r3/decode_avx512.c
                        additional_r3/gf2x_ksqr_avx512.c
                        additional_r3/gf2x_mul_avx512.c
                        additional_r3/sampling_avx512.c)
        set_source_files_properties(${AVX512_SRCS} PROPERTIES COMPILE_FLAGS "-mavx512f -mavx512bw -mavx512dq")
        list(APPEND SRCS_R3 ${AVX512_SRCS})
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_PCLMULQDQ_INSTRUCTIONS)
        set_source_files_properties(additional_r3/gf2x_mul_base_pclmul.c PROPERTIES COMPILE_FLAGS "-mpclmul -msse2")
        list(APPEND SRCS_R3 additional_r3/gf2x_mul_base_pclmul.c)
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_VPCLMULQDQ_INSTRUCTIONS)
        list(APPEND SRCS_R3 additional_r3/gf2x_mul_base_vpclmul.c)

        # Some of our CI configurations use OQS_DIST_BUILD with compilers that do not have
        # the -mvpclmulqdq flag. So we might still have to set DISABLE_VPCLMUL.
        set(vpclmul_supported YES)
        foreach(flag -mvpclmulqdq -mavx512f -mavx512bw -mavx512dq)
          oqs_test_c_flag("${flag}" test)
          if(NOT test)
            set(supported NO)
            break()
          endif()
        endforeach()
        if(vcplmul_supported)
            set_source_files_properties(additional_r3/gf2x_mul_base_vpclmul.c PROPERTIES COMPILE_FLAGS "-mvpclmulqdq -mavx512f -mavx512bw -mavx512dq")
        else()
            list(APPEND CPP_DEFS_R3 DISABLE_VPCLMUL)
        endif()
    else()
        list(APPEND CPP_DEFS_R3 DISABLE_VPCLMUL)
    endif()

    if(OQS_ARCH_X86_64)
        list(APPEND CPP_DEFS_R3 X86_64)
    endif()

    target_include_directories(kem_bike PRIVATE ${CMAKE_CURRENT_LIST_DIR}/functions_renaming.h)

    # Round-3 KEMs
    if(OQS_ENABLE_KEM_bike_l1)
        add_library(bike_l1 OBJECT ${SRCS_R3})
        oqs_target_init(bike_l1)
        oqs_add_c_flags(bike_l1 REQUIRED PRIVATE ${OQS_C_FLAGS})
        oqs_add_ld_flags(bike_l1 PRIVATE ${OQS_LD_FLAGS})
        target_include_directories(bike_l1 PRIVATE ${OQS_INCLUDE_DIRS})

        target_compile_definitions(bike_l1 PRIVATE LEVEL=1 FUNC_PREFIX=OQS_KEM_bike_l1 ${CPP_DEFS_R3})
        target_include_directories(bike_l1 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
        list(APPEND OBJS $<TARGET_OBJECTS:bike_l1>)
    endif()

    if(OQS_ENABLE_KEM_bike_l3)
        add_library(bike_l3 OBJECT ${SRCS_R3})
        oqs_target_init(bike_l3)
        oqs_add_c_flags(bike_l3 REQUIRED PRIVATE ${OQS_C_FLAGS})
        oqs_add_ld_flags(bike_l3 PRIVATE ${OQS_LD_FLAGS})
        target_include_directories(bike_l3 PRIVATE ${OQS_INCLUDE_DIRS})

        target_compile_definitions(bike_l3 PRIVATE LEVEL=3 FUNC_PREFIX=OQS_KEM_bike_l3 ${CPP_DEFS_R3})
        target_include_directories(bike_l3 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
        list(APPEND OBJS $<TARGET_OBJECTS:bike_l3>)
    endif()
endif()

set(OQS_BIKE_OBJS "${OBJS}" PARENT_SCOPE)
