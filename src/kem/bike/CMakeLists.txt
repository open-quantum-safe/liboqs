# SPDX-License-Identifier: MIT

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_C_COMPILER_VERSION VERSION_LESS "5" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    # GCC 4.9 erroneously reports warnings for certain valid code in BIKE
    # https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer
    add_compile_options(-Wno-missing-braces)
    add_compile_options(-Wno-missing-field-initializers)
endif()

set(SRCS additional/decode.c
         additional/sampling.c
         additional/parallel_hash.c
         additional/utilities.c
         additional/aes_ctr_prf.c
         additional/error.c)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(SRCS ${SRCS} additional/sampling_portable.c
                     additional/secure_decode_portable.c)
    set(CPP_DEFS PORTABLE)
elseif(OQS_USE_AVX512_INSTRUCTIONS AND NOT OQS_BUILD_PORTABLE)
    set(SRCS ${SRCS} additional/red.S
                     additional/secure_decode_avx512.c
                     additional/sampling_avx512.S
                     additional/gf_mul.S)
    set(CPP_DEFS AVX512)
    add_compile_options(-mavx512bw -mavx512dq -mavx512f)
elseif(OQS_USE_AVX2_INSTRUCTIONS AND NOT OQS_BUILD_PORTABLE)
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

if(OQS_USE_OPENSSL)
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

add_library(kem_bike OBJECT kem_bike.c)
set(_BIKE_OBJS $<TARGET_OBJECTS:kem_bike>)

# CPA variants
if(OQS_ENABLE_KEM_bike1_l1_cpa)
    add_library(bike1_l1_cpa OBJECT ${SRCS} additional/kem_cpa.c)
    target_compile_definitions(bike1_l1_cpa PRIVATE INDCPA=1 LEVEL=1 FUNC_PREFIX=OQS_KEM_bike1_l1_cpa ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l1_cpa>)
endif()

if(OQS_ENABLE_KEM_bike1_l3_cpa)
    add_library(bike1_l3_cpa OBJECT ${SRCS} additional/kem_cpa.c)
    target_compile_definitions(bike1_l3_cpa PRIVATE INDCPA=1 LEVEL=3 FUNC_PREFIX=OQS_KEM_bike1_l3_cpa ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l3_cpa>)
endif()

# FO variants
if(OQS_ENABLE_KEM_bike1_l1_fo)
    add_library(bike1_l1_fo OBJECT ${SRCS} additional/kem.c)
    target_compile_definitions(bike1_l1_fo PRIVATE LEVEL=1 FUNC_PREFIX=OQS_KEM_bike1_l1_fo ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l1_fo>)
endif()

if(OQS_ENABLE_KEM_bike1_l3_fo)
    add_library(bike1_l3_fo OBJECT ${SRCS} additional/kem.c)
    target_compile_definitions(bike1_l3_fo PRIVATE LEVEL=3 FUNC_PREFIX=OQS_KEM_bike1_l3_fo ${CPP_DEFS})
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l3_fo>)
endif()

set(BIKE_OBJS ${_BIKE_OBJS} PARENT_SCOPE)
