cmake_minimum_required (VERSION 3.12)

project(liboqs NONE)
set(OQS_VERSION_TEXT "0.2.0-dev")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")

enable_language(C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR
   ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
    set(ARCH "x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
    # cmake reports AMD64 on Windows, but we might be building for 32-bit.
    if(CMAKE_CL_64)
        set(ARCH "x86_64")
    else()
        set(ARCH "x86")
    endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86" OR
       ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386" OR
       ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
    set(ARCH "x86")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
    set(ARCH "arm64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    set(ARCH "arm")
else()
    message(FATAL_ERROR "Unknown or unsupported processor:" ${CMAKE_SYSTEM_PROCESSOR})
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
endif()

include(config.cmake)

if(USE_OPENSSL)
    if(NOT DEFINED OPENSSL_ROOT_DIR)
        if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
            set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1")
        elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
            set(OPENSSL_ROOT_DIR "/usr")
        endif()
    endif()
    find_package(OpenSSL REQUIRED)
endif()

set(HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
            ${PROJECT_SOURCE_DIR}/src/common/common.h
            ${PROJECT_SOURCE_DIR}/src/common/rand.h
            ${PROJECT_SOURCE_DIR}/src/crypto/aes/aes.h
            ${PROJECT_SOURCE_DIR}/src/crypto/sha2/sha2.h
            ${PROJECT_SOURCE_DIR}/src/crypto/sha3/sha3.h
            ${PROJECT_SOURCE_DIR}/src/kem/kem.h)
if(ENABLE_KEM_BIKE)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/bike/kem_bike.h)
endif()
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
if(ENABLE_KEM_SIKE)
    set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/sike/kem_sike.h)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)

configure_file(oqsconfig.h.cmake ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)

include_directories(${PROJECT_BINARY_DIR}/include)

if(BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()
add_subdirectory(src)

add_subdirectory(tests)
