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

set(OSSL_HELPERS "")

if(${OQS_USE_AES_OPENSSL})
    set(AES_IMPL aes/aes_ossl.c)
    set(OSSL_HELPERS ossl_helpers.c)
else()
   set(AES_IMPL aes/aes.c aes/aes_c.c)
   if (OQS_DIST_X86_64_BUILD OR OQS_USE_AES_INSTRUCTIONS)
      set(AES_IMPL ${AES_IMPL} aes/aes128_ni.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_ni.c)
      set_source_files_properties(aes/aes128_ni.c PROPERTIES COMPILE_FLAGS -maes)
      set_source_files_properties(aes/aes256_ni.c PROPERTIES COMPILE_FLAGS "-maes -mssse3")
   elseif (OQS_DIST_ARM64_V8_BUILD)
      set(AES_IMPL ${AES_IMPL} aes/aes128_armv8.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_armv8.c)
      set_source_files_properties(aes/aes128_armv8.c PROPERTIES COMPILE_FLAGS -mcpu=cortex-a53+crypto)
      set_source_files_properties(aes/aes256_armv8.c PROPERTIES COMPILE_FLAGS -mcpu=cortex-a53+crypto)
   elseif (OQS_USE_ARM_AES_INSTRUCTIONS)
      set(AES_IMPL ${AES_IMPL} aes/aes128_armv8.c)
      set(AES_IMPL ${AES_IMPL} aes/aes256_armv8.c)
       if ((CMAKE_SYSTEM_NAME MATCHES "Darwin") AND (${CMAKE_C_COMPILER_ID} STREQUAL "GNU"))
         set_source_files_properties(aes/aes128_armv8.c PROPERTIES COMPILE_FLAGS -march=armv8-a+crypto)
         set_source_files_properties(aes/aes256_armv8.c PROPERTIES COMPILE_FLAGS -march=armv8-a+crypto)
       endif()
   endif()
endif()

if(${OQS_USE_SHA2_OPENSSL})
    set(SHA2_IMPL sha2/sha2_ossl.c)
    set(OSSL_HELPERS ossl_helpers.c)
else()
    set(SHA2_IMPL sha2/sha2.c sha2/sha2_c.c)
    if (OQS_DIST_ARM64_V8_BUILD)
       set(SHA2_IMPL ${SHA2_IMPL} sha2/sha2_armv8.c)
       set_source_files_properties(sha2/sha2_armv8.c PROPERTIES COMPILE_FLAGS -mcpu=cortex-a53+crypto)
    elseif (OQS_USE_ARM_SHA2_INSTRUCTIONS)
       # Assume we are compiling native
       set(SHA2_IMPL ${SHA2_IMPL} sha2/sha2_armv8.c)
       if ((CMAKE_SYSTEM_NAME MATCHES "Darwin") AND (${CMAKE_C_COMPILER_ID} STREQUAL "GNU"))
         set_source_files_properties(sha2/sha2_armv8.c PROPERTIES COMPILE_FLAGS -march=armv8-a+crypto)
       endif()
    endif()
endif()

if(${OQS_USE_SHA3_OPENSSL})
  if (${OQS_ENABLE_SHA3_xkcp_low})
    add_subdirectory(sha3/xkcp_low)
  endif()
    set(SHA3_IMPL sha3/ossl_sha3.c sha3/ossl_sha3x4.c)
    set(OSSL_HELPERS ossl_helpers.c)
else() # using XKCP
    add_subdirectory(sha3/xkcp_low)
    set(SHA3_IMPL sha3/xkcp_sha3.c sha3/xkcp_sha3x4.c)
endif()

add_library(common OBJECT ${AES_IMPL}
                          ${SHA2_IMPL}
                          ${SHA3_IMPL}
                          ${OSSL_HELPERS}
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
if(CMAKE_USE_PTHREADS_INIT)
    target_link_libraries(common PRIVATE Threads::Threads)
endif()

# check available functions to perform aligned mallocs
check_symbol_exists(aligned_alloc stdlib.h CMAKE_HAVE_ALIGNED_ALLOC)
check_symbol_exists(posix_memalign stdlib.h CMAKE_HAVE_POSIX_MEMALIGN)
check_symbol_exists(memalign malloc.h CMAKE_HAVE_MEMALIGN)

if(CMAKE_HAVE_ALIGNED_ALLOC)
    target_compile_definitions(common PRIVATE OQS_HAVE_ALIGNED_ALLOC)
endif()

if(CMAKE_HAVE_POSIX_MEMALIGN)
    target_compile_definitions(common PRIVATE OQS_HAVE_POSIX_MEMALIGN)
endif()

if(CMAKE_HAVE_MEMALIGN)
    target_compile_definitions(common PRIVATE OQS_HAVE_MEMALIGN)
endif()

# check if explicit_bzero exists or memset_s
check_symbol_exists(explicit_bzero string.h CMAKE_HAVE_EXPLICIT_BZERO)
check_symbol_exists(memset_s string.h CMAKE_HAVE_MEMSET_S)

if(CMAKE_HAVE_EXPLICIT_BZERO)
    target_compile_definitions(common PRIVATE OQS_HAVE_EXPLICIT_BZERO)
endif()

if(CMAKE_HAVE_MEMSET_S)
    target_compile_definitions(common PRIVATE OQS_HAVE_MEMSET_S)
endif()

if(${OQS_ENABLE_SHA3_xkcp_low}) # using XKCP
    set(_COMMON_OBJS ${_COMMON_OBJS} ${XKCP_LOW_OBJS})
endif()

set(_COMMON_OBJS ${_COMMON_OBJS} $<TARGET_OBJECTS:common>)
set(COMMON_OBJS ${_COMMON_OBJS} PARENT_SCOPE)
