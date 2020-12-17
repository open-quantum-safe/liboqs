# SPDX-License-Identifier: MIT

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wbad-function-cast)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wnarrowing)
    add_compile_options(-Wconversion)
endif()

if(OQS_USE_AES_OPENSSL)
    set(AES_IMPL aes/aes_ossl.c)
else()
   set(AES_IMPL aes/aes.c aes/aes_c.c)
    if (OQS_USE_AES_INSTRUCTIONS)
      set(AES_IMPL ${AES_IMPL} aes/aes128_ni.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_ni.c)
      set_source_files_properties(aes/aes128_ni.c PROPERTIES COMPILE_FLAGS -maes)
      set_source_files_properties(aes/aes256_ni.c PROPERTIES COMPILE_FLAGS -maes)
   endif()
endif()

if(OQS_USE_SHA2_OPENSSL)
    set(SHA2_IMPL sha2/sha2_ossl.c)
else()
    set(SHA2_IMPL sha2/sha2_c.c)
endif()

set(SHA3_IMPL sha3/sha3_c.c)
if(OQS_USE_SHA3_OPENSSL)
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3_ossl.c)
endif()

# If the code below is compiled in and run, the CPU will not have a
# fallback in case it does not support AVX2.

# At the moment, this cannot be helped: if AVX2 compilation is supported and
# OQS_PORTABLE_BUILD is set, Kyber, SPHINCS, and Dilithium compile in code that
# includes KeccakP1600times4_PermuteAll_24rounds from
# src/common/sha3/keccak4x/KeccakP-1600-times4-SIMD256.c

# However, this shouldn't affect portability since when OQS_PORTABLE_BUILD
# is specified, the algorithms fallback to non-AVX2 code when necessary, which
# does not make calls to the SHA3 AVX2 functions, which in turn means that the
# inclusion of SHA3 AVX2 code in the binary even when OQS_PORTABLE_BUILD is
# specified should not be a problem.

# That being said, code that requires these functions must take care to ensure
# that they fall-back appropriately.
# What must be ensured, though, is that only sha3x4 is compiled with avx2;
# all other code must be compiled without this extension if portability is requested
if(OQS_USE_AVX2_INSTRUCTIONS)
if(OQS_PORTABLE_BUILD)
    add_library(sha3x4_avx2 OBJECT sha3/sha3x4.c)
    target_compile_options(sha3x4_avx2 PRIVATE -mavx2)
    set(_COMMON_OBJS $<TARGET_OBJECTS:sha3x4_avx2>)
else()
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3x4.c)
    add_compile_options(-mavx2)
endif()
endif()

add_library(common OBJECT ${AES_IMPL}
                          ${SHA2_IMPL}
                          ${SHA3_IMPL}
                          common.c
                          pqclean_shims/nistseedexpander.c
                          rand/rand.c
                          rand/rand_nist.c)
if(OQS_USE_OPENSSL)
    target_include_directories(common PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()

set(_COMMON_OBJS ${_COMMON_OBJS} $<TARGET_OBJECTS:common>)
set(COMMON_OBJS ${_COMMON_OBJS} PARENT_SCOPE)
