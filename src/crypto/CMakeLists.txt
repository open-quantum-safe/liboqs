if(USE_AES_OPENSSL)
    set(AES_IMPL aes/aes_ossl.c)
else()
    set(AES_IMPL aes/aes_c.c)
endif()

if(USE_SHA2_OPENSSL)
    set(SHA2_IMPL sha2/sha2_ossl.c)
else()
    set(SHA2_IMPL sha2/sha2_c.c)
endif()

set(SHA3_IMPL sha3/sha3_c.c)
if(USE_SHA3_OPENSSL)
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3_ossl.c)
endif()
if(USE_AVX2_INSTRUCTIONS AND USE_AES_INSTRUCTIONS)
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3x4.c)
endif()

# We prepend absolute paths as the variables
# are needed in the tests/ sub-directory.
list(TRANSFORM AES_IMPL PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
list(TRANSFORM SHA2_IMPL PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
list(TRANSFORM SHA3_IMPL PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(crypto OBJECT ${AES_IMPL} ${SHA2_IMPL} ${SHA3_IMPL})
