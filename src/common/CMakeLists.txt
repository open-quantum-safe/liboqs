# SPDX-License-Identifier: MIT

include(CheckSymbolExists)

# initialize to avoid --warn-uninitialized report
set(_COMMON_OBJS "")

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wbad-function-cast)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wnarrowing)
    add_compile_options(-Wconversion)
endif()

if(${OQS_USE_AES_OPENSSL})
    set(AES_IMPL aes/aes_ossl.c)
else()
   set(AES_IMPL aes/aes.c aes/aes_c.c)
   if (OQS_DIST_X86_64_BUILD OR OQS_USE_AES_INSTRUCTIONS)
      set(AES_IMPL ${AES_IMPL} aes/aes128_ni.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_ni.c)
      set_source_files_properties(aes/aes128_ni.c PROPERTIES COMPILE_FLAGS -maes)
      set_source_files_properties(aes/aes256_ni.c PROPERTIES COMPILE_FLAGS -maes)
   elseif (OQS_USE_ARM_AES_INSTRUCTIONS)
      set(AES_IMPL ${AES_IMPL} aes/aes128_armv8.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_armv8.c)
   endif()
endif()

if(${OQS_USE_SHA2_OPENSSL})
    set(SHA2_IMPL sha2/sha2_ossl.c)
else()
    set(SHA2_IMPL sha2/sha2.c sha2/sha2_c.c)
    if (OQS_DIST_ARM64v8_BUILD)
       set(SHA2_IMPL ${SHA2_IMPL} sha2/sha2_ni.c)
       set_source_files_properties(sha2/sha2_ni.c PROPERTIES COMPILE_FLAGS -mcpu=cortex-a53+crypto)
    elseif (OQS_USE_ARM_SHA2_INSTRUCTIONS)
       # Assume we are compiling native
       set(SHA2_IMPL ${SHA2_IMPL} sha2/sha2_ni.c)
    endif()
endif()

if(${OQS_USE_SHA3_OPENSSL})
    set(SHA3_IMPL sha3/ossl_sha3.c sha3/ossl_sha3x4.c)
else() # using XKCP
    add_subdirectory(sha3/xkcp_low)
    set(SHA3_IMPL sha3/xkcp_sha3.c sha3/xkcp_sha3x4.c)
endif()

add_library(common OBJECT ${AES_IMPL}
                          ${SHA2_IMPL}
                          ${SHA3_IMPL}
                          common.c
                          pqclean_shims/nistseedexpander.c
                          pqclean_shims/fips202.c
                          pqclean_shims/fips202x4.c
                          rand/rand.c
                          rand/rand_nist.c)

if(${OQS_USE_OPENSSL})
    target_include_directories(common PRIVATE ${OPENSSL_INCLUDE_DIR})
else()
    check_symbol_exists(getentropy "unistd.h;sys/random.h" CMAKE_HAVE_GETENTROPY)
    if(${CMAKE_HAVE_GETENTROPY})
        target_compile_definitions(common PRIVATE OQS_HAVE_GETENTROPY)
    endif()
endif()

if(NOT ${OQS_USE_SHA3_OPENSSL}) # using XKCP
    set(_COMMON_OBJS ${_COMMON_OBJS} ${XKCP_LOW_OBJS})
endif()

set(_COMMON_OBJS ${_COMMON_OBJS} $<TARGET_OBJECTS:common>)
set(COMMON_OBJS ${_COMMON_OBJS} PARENT_SCOPE)
