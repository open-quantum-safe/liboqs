cmake_minimum_required (VERSION 3.5)
cmake_policy (SET CMP0054 NEW)

project(liboqs
        VERSION 0.2.0
        HOMEPAGE_URL https://openquantumsafe.org)

set(CMAKE_VERBOSE_MAKEFILE ON)
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Out of source build required")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
if (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-g -Wall -mavx2)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    add_compile_options(-g pedantic -Wall -Wextra)
endif ()

set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
find_package(OpenSSL REQUIRED)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/oqs.h
        ${CMAKE_SOURCE_DIR}/src/oqsconfig.h
        ${CMAKE_SOURCE_DIR}/src/common/common.h
        ${CMAKE_SOURCE_DIR}/src/common/rand.h
        ${CMAKE_SOURCE_DIR}/src/crypto/aes/aes.h
        ${CMAKE_SOURCE_DIR}/src/crypto/sha2/sha2.h
        ${CMAKE_SOURCE_DIR}/src/crypto/sha3/sha3.h
        ${CMAKE_SOURCE_DIR}/src/kem/kem.h
        ${CMAKE_SOURCE_DIR}/src/kem/frodokem/kem_frodokem.h
        ${CMAKE_SOURCE_DIR}/src/kem/kyber/kem_kyber.h
        ${PROJECT_BINARY_DIR}/include/oqs)
include_directories(${PROJECT_BINARY_DIR}/include)

option(USE_AES_OPENSSL "Use OpenSSL's AES implementation." ON)
option(USE_SHA2_OPENSSL "Use OpenSSL's SHA2 implementation." ON)
option(USE_SHA3_OPENSSL "Use OpenSSL's SHA3 implementation." ON)
option(USE_AVX2_INSTRUCTIONS "Use AVX assembly instructions./" ON)
option(USE_AES_INSTRUCTIONS "Use AES-NI assembly instructions." ON)
add_subdirectory(src)

add_subdirectory(tests)
