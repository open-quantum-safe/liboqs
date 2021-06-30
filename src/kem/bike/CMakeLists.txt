# SPDX-License-Identifier: MIT

if(OQS_ENABLE_KEM_bike_l1 OR OQS_ENABLE_KEM_bike_l3)
    set(_BIKE_OBJS "")
    set(CPP_DEFS_R3 "")

    add_library(kem_bike OBJECT kem_bike.c)
    set(_BIKE_OBJS $<TARGET_OBJECTS:kem_bike>)

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
        set(SRCS_R3 ${SRCS_R3} ${AVX2_SRCS})
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_AVX512_INSTRUCTIONS)
        set(AVX512_SRCS additional_r3/gf2x_mul_base_vpclmul.c
                        additional_r3/decode_avx512.c
                        additional_r3/gf2x_ksqr_avx512.c
                        additional_r3/gf2x_mul_avx512.c
                        additional_r3/sampling_avx512.c)
        set_source_files_properties(${AVX512_SRCS} PROPERTIES COMPILE_FLAGS "-mavx512f -mavx512bw -mavx512dq")
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_PCLMULQDQ_INSTRUCTIONS)
        set_source_files_properties(additional_r3/gf2x_mul_base_pclmul.c PROPERTIES COMPILE_FLAGS "-mpclmul -msse2")
        set(SRCS_R3 ${SRCS_R3} additional_r3/gf2x_mul_base_pclmul.c)
    endif()

    if (OQS_DIST_X86_64_BUILD OR OQS_USE_VPCLMULQDQ_INSTRUCTIONS)
        set_source_files_properties(additional_r3/gf2x_mul_base_vpclmul.c PROPERTIES COMPILE_FLAGS "-mvpclmulqdq -mavx512f -mavx512bw -mavx512dq")
        set(SRCS_R3 ${SRCS_R3} additional_r3/gf2x_mul_base_vpclmul.c)
    endif()

    if(ARCH_X86_64)
        set(CPP_DEFS_R3 ${CPP_DEFS_R3} X86_64)
    endif()

    add_compile_options(-include ${CMAKE_CURRENT_LIST_DIR}/functions_renaming.h)

    # Round-3 KEMs
    if(OQS_ENABLE_KEM_bike_l1)
        add_library(bike_l1 OBJECT ${SRCS_R3} additional_r3/kem.c)
        target_compile_definitions(bike_l1 PRIVATE LEVEL=1 FUNC_PREFIX=OQS_KEM_bike_l1 ${CPP_DEFS_R3})
        target_include_directories(bike_l1 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
        set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike_l1>)
    endif()

    if(OQS_ENABLE_KEM_bike_l3)
        add_library(bike_l3 OBJECT ${SRCS_R3} additional_r3/kem.c)
        target_compile_definitions(bike_l3 PRIVATE LEVEL=3 FUNC_PREFIX=OQS_KEM_bike_l3 ${CPP_DEFS_R3})
        target_include_directories(bike_l3 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
        set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike_l3>)
    endif()
endif()

set(BIKE_OBJS ${_BIKE_OBJS} PARENT_SCOPE)
