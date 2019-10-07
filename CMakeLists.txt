cmake_minimum_required (VERSION 3.0)

project(liboqs NONE)
enable_language(C)
set(OQS_VERSION_TEXT "0.2.0-dev")

if(NOT ${CMAKE_C_COMPILER_ID} MATCHES "MSVC")
    add_compile_options(-g -Wall)
endif()

include(config.cmake)
if(USE_AVX2_INSTRUCTIONS)
    add_compile_options(-mavx2)
endif()

if(USE_OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()

set(HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
            ${PROJECT_SOURCE_DIR}/src/common/common.h
            ${PROJECT_SOURCE_DIR}/src/common/rand.h
            ${PROJECT_SOURCE_DIR}/src/crypto/aes/aes.h
            ${PROJECT_SOURCE_DIR}/src/crypto/sha2/sha2.h
            ${PROJECT_SOURCE_DIR}/src/crypto/sha3/sha3.h
            ${PROJECT_SOURCE_DIR}/src/kem/kem.h)
if(ENABLE_KEM_FRODOKEM)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/frodokem/kem_frodokem.h)
endif()
if(ENABLE_KEM_KYBER)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/kyber/kem_kyber.h)
endif()
if(ENABLE_KEM_NEWHOPE)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/newhope/kem_newhope.h)
endif()
if(ENABLE_KEM_NTRU)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntru/kem_ntru.h)
endif()
if(ENABLE_KEM_SABER)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/saber/kem_saber.h)
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
configure_file(oqsconfig.h.cmake ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
include_directories(${PROJECT_BINARY_DIR}/include)

if(BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()
add_subdirectory(src)

add_subdirectory(tests)
