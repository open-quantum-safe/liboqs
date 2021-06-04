# SPDX-License-Identifier: MIT

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_C_COMPILER_VERSION VERSION_LESS "5" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    # GCC 4.9 erroneously reports warnings for certain valid code in BIKE
    # https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer
    add_compile_options(-Wno-missing-braces)
    add_compile_options(-Wno-missing-field-initializers)
endif()

if(OQS_ENABLE_KEM_bike1_l1_cpa OR OQS_ENABLE_KEM_bike1_l3_cpa OR
   OQS_ENABLE_KEM_bike1_l1_fo OR OQS_ENABLE_KEM_bike1_l3_fo)

    set(SRCS additional/decode.c
             additional/sampling.c
             additional/parallel_hash.c
             additional/utilities.c
             additional/aes_ctr_prf.c
             additional/error.c)
endif()

if(OQS_ENABLE_KEM_bike_l1 OR OQS_ENABLE_KEM_bike_l3)

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

    if(ARCH_X86_64)
        set(SRCS_R3 ${SRCS_R3} additional_r3/decode_avx2.c
                               additional_r3/decode_avx512.c
                               additional_r3/gf2x_ksqr_avx2.c
                               additional_r3/gf2x_ksqr_avx512.c
                               additional_r3/gf2x_mul_avx2.c
                               additional_r3/gf2x_mul_avx512.c
                               additional_r3/gf2x_mul_base_pclmul.c
                               additional_r3/gf2x_mul_base_vpclmul.c
                               additional_r3/sampling_avx2.c
                               additional_r3/sampling_avx512.c)

        set(CPP_DEFS_R3 X86_64)

        FILE(GLOB_RECURSE AVX2_SRCS additional_r3/*_avx2.c)
        FILE(GLOB_RECURSE AVX512_SRCS additional_r3/*_avx512.c)

        set(AVX512_FLAGS "-mavx512f;-mavx512bw;-mavx512dq")

        # Set appropriate flags for avx files
        set_source_files_properties(${AVX2_SRCS} PROPERTIES COMPILE_OPTIONS "-mavx2")
        set_source_files_properties(${AVX512_SRCS} PROPERTIES COMPILE_OPTIONS "${AVX512_FLAGS}")

        set_source_files_properties(additional_r3/gf2x_mul_base_pclmul.c PROPERTIES COMPILE_OPTIONS "-mpclmul;-msse2")
        set_source_files_properties(additional_r3/gf2x_mul_base_vpclmul.c PROPERTIES COMPILE_OPTIONS "-mvpclmulqdq;${AVX512_FLAGS}")
    endif()

endif()

if ( (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin") OR
     (CMAKE_HOST_SYSTEM_NAME STREQUAL "CYGWIN") )
    set(SRCS ${SRCS} additional/sampling_portable.c
                     additional/secure_decode_portable.c)
    set(CPP_DEFS PORTABLE)
elseif(OQS_USE_AVX512_INSTRUCTIONS AND NOT ${OQS_DIST_BUILD})
    set(SRCS ${SRCS} additional/red.S
                     additional/secure_decode_avx512.c
                     additional/sampling_avx512.S
                     additional/gf_mul.S)
    set(CPP_DEFS AVX512)
    add_compile_options(-mavx512bw -mavx512dq -mavx512f)
elseif(OQS_USE_AVX2_INSTRUCTIONS AND NOT ${OQS_DIST_BUILD})
    set(SRCS ${SRCS} additional/red.S
                     additional/secure_decode_avx2.c
                     additional/sampling_avx2.S
                     additional/gf_mul.S)
    set(CPP_DEFS AVX2)
    add_compile_options(-mavx2)
else()
    set(SRCS ${SRCS} additional/sampling_portable.c
                     additional/secure_decode_portable.c)
    set(CPP_DEFS PORTABLE)
endif()

add_compile_options(-include ${CMAKE_CURRENT_LIST_DIR}/functions_renaming.h)

if(${OQS_USE_OPENSSL})
    # TODO: Find a way to eliminate this include_directories() call
    include_directories(${OPENSSL_INCLUDE_DIR})
    set(SRCS ${SRCS} additional/openssl_utils.c)
    include(CheckSymbolExists)
    check_symbol_exists(OPENSSL_NO_EC2M "openssl/opensslconf.h" OPENSSL_NO_EC2M_SET)
    if(OPENSSL_NO_EC2M_SET)
       set(SRCS ${SRCS} additional/gf2x_portable.c additional/gf2x_mul.c)
    endif()
else()
    set(SRCS ${SRCS} additional/gf2x_portable.c additional/gf2x_mul.c)
endif()

# CPA variants
if(OQS_ENABLE_KEM_bike1_l1_cpa)
    add_library(bike1_l1_cpa OBJECT kem_bike.c ${SRCS} additional/kem_cpa.c)
    target_compile_definitions(bike1_l1_cpa PRIVATE INDCPA=1 LEVEL=1 FUNC_PREFIX=OQS_KEM_bike1_l1_cpa ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l1_cpa>)
endif()

if(OQS_ENABLE_KEM_bike1_l3_cpa)
    add_library(bike1_l3_cpa OBJECT kem_bike.c ${SRCS} additional/kem_cpa.c)
    target_compile_definitions(bike1_l3_cpa PRIVATE INDCPA=1 LEVEL=3 FUNC_PREFIX=OQS_KEM_bike1_l3_cpa ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l3_cpa>)
endif()

# FO variants
if(OQS_ENABLE_KEM_bike1_l1_fo)
    add_library(bike1_l1_fo OBJECT kem_bike.c ${SRCS} additional/kem.c)
    target_compile_definitions(bike1_l1_fo PRIVATE LEVEL=1 FUNC_PREFIX=OQS_KEM_bike1_l1_fo ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l1_fo>)
endif()

if(OQS_ENABLE_KEM_bike1_l3_fo)
    add_library(bike1_l3_fo OBJECT kem_bike.c ${SRCS} additional/kem.c)
    target_compile_definitions(bike1_l3_fo PRIVATE LEVEL=3 FUNC_PREFIX=OQS_KEM_bike1_l3_fo ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l3_fo>)
endif()

# Round-3 KEMs
if(OQS_ENABLE_KEM_bike_l1)
    add_library(bike_l1 OBJECT kem_bike.c ${SRCS_R3} additional_r3/kem.c)
    target_compile_definitions(bike_l1 PRIVATE LEVEL=1 FUNC_PREFIX=OQS_KEM_bike_l1 ${CPP_DEFS_R3})
    target_include_directories(bike_l1 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike_l1>)
endif()

if(OQS_ENABLE_KEM_bike_l3)
    add_library(bike_l3 OBJECT kem_bike.c ${SRCS_R3} additional_r3/kem.c)
    target_compile_definitions(bike_l3 PRIVATE LEVEL=3 FUNC_PREFIX=OQS_KEM_bike_l3 ${CPP_DEFS_R3})
    target_include_directories(bike_l3 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/additional_r3)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike_l3>)
endif()

set(BIKE_OBJS ${_BIKE_OBJS} PARENT_SCOPE)
