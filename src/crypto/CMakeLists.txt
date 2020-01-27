# The *_IMPL variables are absolute paths
# as they are needed in the tests/ sub-directory.
if(USE_AES_OPENSSL)
    set(AES_IMPL ${CMAKE_CURRENT_LIST_DIR}/aes/aes_ossl.c)
else()
    set(AES_IMPL ${CMAKE_CURRENT_LIST_DIR}/aes/aes_c.c)
endif()

if(USE_SHA2_OPENSSL)
    set(SHA2_IMPL ${CMAKE_CURRENT_LIST_DIR}/sha2/sha2_ossl.c)
else()
    set(SHA2_IMPL ${CMAKE_CURRENT_LIST_DIR}/sha2/sha2_c.c)
endif()

set(SHA3_IMPL ${CMAKE_CURRENT_LIST_DIR}/sha3/sha3_c.c)
if(USE_SHA3_OPENSSL)
    set(SHA3_IMPL ${SHA3_IMPL} ${CMAKE_CURRENT_LIST_DIR}/sha3/sha3_ossl.c)
endif()
if(USE_AVX2_INSTRUCTIONS AND USE_AES_INSTRUCTIONS)
    set(SHA3_IMPL ${SHA3_IMPL} ${CMAKE_CURRENT_LIST_DIR}/sha3/sha3x4.c)
endif()

add_library(crypto OBJECT ${AES_IMPL} ${SHA2_IMPL} ${SHA3_IMPL})

if(USE_OPENSSL)
    target_include_directories(crypto PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()
