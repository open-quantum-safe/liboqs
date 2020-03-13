include(crypto_impl.cmake)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wbad-function-cast)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wnarrowing)
    add_compile_options(-Wconversion)
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
