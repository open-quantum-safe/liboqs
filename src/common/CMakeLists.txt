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
    set(AES_IMPL aes/aes_c.c)
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
if(OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_AES_INSTRUCTIONS)
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3x4.c)
    add_compile_options(-mavx2)
    add_compile_options(-maes)
endif()

add_library(common OBJECT ${AES_IMPL}
                          ${SHA2_IMPL}
                          ${SHA3_IMPL}
                          common.c
                          rand/rand.c
                          rand/rand_nist.c)
if(OQS_USE_OPENSSL)
    target_include_directories(common PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()
