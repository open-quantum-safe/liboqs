if(USE_AVX2_INSTRUCTIONS)
    set(ROOT "x86_64")
else()
    set(ROOT "ref")
endif()

set(SRCS kem_bike.c
         ${ROOT}/kem.c
         ${ROOT}/aes_ctr_prf.c
         ${ROOT}/parallel_hash.c
         ${ROOT}/sampling.c
         ${ROOT}/utilities.c
         ${ROOT}/decode.c
         ${ROOT}/openssl_utils.c)

if(USE_AVX2_INSTRUCTIONS)
    set(SRCS ${SRCS} ${ROOT}/aes.c
                     ${ROOT}/sha.c
                     ${ROOT}/sha-mb.c
                     ${ROOT}/gf2x_mul.c
                     ${ROOT}/vaes256_key_expansion.S
                     ${ROOT}/gf_add.S
                     ${ROOT}/converts.S
                     ${ROOT}/gf_mul.S
                     ${ROOT}/sha384_update_mb.S
                     ${ROOT}/red.S
                     ${ROOT}/sampling_x86_64.S
                     ${ROOT}/decode_x86_64.S)
else()
    set(SRCS ${SRCS} ${ROOT}/conversions.c)
endif()

add_compile_options(-include ${CMAKE_CURRENT_LIST_DIR}/functions_renaming.h)
add_compile_definitions(CONSTANT_TIME)

# L1 variants
if(OQS_ENABLE_KEM_bike1_l1)
    add_library(bike1_l1 OBJECT ${SRCS})
    target_compile_definitions(bike1_l1 PRIVATE LEVEL=1 BIKE_VER=1 FUNC_PREFIX=OQS_KEM_bike1_l1)
    set(_BIKE_OBJS $<TARGET_OBJECTS:bike1_l1>)
endif()

if(OQS_ENABLE_KEM_bike2_l1)
    add_library(bike2_l1 OBJECT ${SRCS})
    target_compile_definitions(bike2_l1 PRIVATE LEVEL=1 BIKE_VER=2 FUNC_PREFIX=OQS_KEM_bike2_l1)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike2_l1>)
endif()

if(OQS_ENABLE_KEM_bike3_l1)
    add_library(bike3_l1 OBJECT ${SRCS})
    target_compile_definitions(bike3_l1 PRIVATE LEVEL=1 BIKE_VER=3 FUNC_PREFIX=OQS_KEM_bike3_l1)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike3_l1>)
endif()

# L3 variants
if(OQS_ENABLE_KEM_bike1_l3)
    add_library(bike1_l3 OBJECT ${SRCS})
    target_compile_definitions(bike1_l3 PRIVATE LEVEL=3 BIKE_VER=1 FUNC_PREFIX=OQS_KEM_bike1_l3)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l3>)
endif()

if(OQS_ENABLE_KEM_bike2_l3)
    add_library(bike2_l3 OBJECT ${SRCS})
    target_compile_definitions(bike2_l3 PRIVATE LEVEL=3 BIKE_VER=2 FUNC_PREFIX=OQS_KEM_bike2_l3)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike2_l3>)
endif()

if(OQS_ENABLE_KEM_bike3_l3)
    add_library(bike3_l3 OBJECT ${SRCS})
    target_compile_definitions(bike3_l3 PRIVATE LEVEL=3 BIKE_VER=3 FUNC_PREFIX=OQS_KEM_bike3_l3)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike3_l3>)
endif()

# L5 variants
if(OQS_ENABLE_KEM_bike1_l5)
    add_library(bike1_l5 OBJECT ${SRCS})
    target_compile_definitions(bike1_l5 PRIVATE LEVEL=5 BIKE_VER=1 FUNC_PREFIX=OQS_KEM_bike1_l5)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike1_l5>)
endif()

if(OQS_ENABLE_KEM_bike2_l5)
    add_library(bike2_l5 OBJECT ${SRCS})
    target_compile_definitions(bike2_l5 PRIVATE LEVEL=5 BIKE_VER=2 FUNC_PREFIX=OQS_KEM_bike2_l5)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike2_l5>)
endif()

if(OQS_ENABLE_KEM_bike3_l5)
    add_library(bike3_l5 OBJECT ${SRCS})
    target_compile_definitions(bike3_l5 PRIVATE LEVEL=5 BIKE_VER=3 FUNC_PREFIX=OQS_KEM_bike3_l5)
    set(_BIKE_OBJS ${_BIKE_OBJS} $<TARGET_OBJECTS:bike3_l5>)
endif()

set(BIKE_OBJS ${_BIKE_OBJS} PARENT_SCOPE)
