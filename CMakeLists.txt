# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.5)
# option() honors normal variables.
# see: https://cmake.org/cmake/help/git-stage/policy/CMP0077.html
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
# Honor symbol visibility properties for all target types.
# see: https://cmake.org/cmake/help/git-stage/policy/CMP0063.html
if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

project(liboqs C ASM)

option(OQS_DIST_BUILD "Build distributable library with optimized code for several CPU microarchitectures. Enables run-time CPU feature detection." OFF)
option(OQS_BUILD_ONLY_LIB "Build only liboqs and do not expose build targets for tests, documentation, and pretty-printing available." OFF)
set(OQS_MINIMAL_BUILD "" CACHE STRING "Only build specifically listed algorithms.")
option(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE "Permit compilation on an an unsupported architecture." OFF)

set(OQS_OPT_TARGET auto CACHE STRING "The target microarchitecture for optimization.")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(OQS_VERSION_TEXT "0.7.1")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    set(ARCH "x86_64")
    set(ARCH_X86_64 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_X86_64_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i586|i686")
    set(ARCH "i586")
    set(ARCH_X86 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_X86_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm64v8")
    set(ARCH "arm64v8")
    set(ARCH_ARM64v8 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_ARM64v8_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armel|armhf|armv7|arm32v7")
    set(ARCH "arm32v7")
    set(ARCH_ARM32v7 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_ARM32v7_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ppc64le|powerpc64le")
    set(ARCH "ppc64le")
    set(ARCH_PPC64LE ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_PPC64LE_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "s390x")
    set(ARCH "s390x")
    set(ARCH_S390X ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_S390X_BUILD ON)
    endif()
elseif(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE)
    message(WARNING "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR})
    message(WARNING "Compilation on an unsupported processor should only be used for testing, as it may result an insecure configuration, for example due to variable-time instructions leaking secret information.")
    # disable SIKE whose build fails on unsupported platforms
    set(OQS_ENABLE_KEM_SIKE OFF)
    set(OQS_ENABLE_KEM_SIDH OFF)
else()
    message(FATAL_ERROR "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR} ". Override by setting OQS_PERMIT_UNSUPPORTED_ARCHITECTURE=ON")
endif()

# intentionally don't switch to variables to avoid --warn-uninitialized report
if(OQS_USE_CPU_EXTENSIONS)
    message(FATAL_ERROR "OQS_USE_CPU_EXTENSIONS is deprecated")
endif()

# intentionally don't switch to variables to avoid --warn-uninitialized report
if(OQS_PORTABLE_BUILD)
    message(FATAL_ERROR "OQS_PORTABLE_BUILD is deprecated")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(OQS_DEBUG_BUILD ON)
else()
    set(OQS_DEBUG_BUILD OFF)
endif()

option(OQS_SPEED_USE_ARM_PMU "Use ARM Performance Monitor Unit during benchmarking" OFF)

if(WIN32)
    set(CMAKE_GENERATOR_CC cl)
endif()

include(.CMake/compiler_opts.cmake)
include(.CMake/alg_support.cmake)

if(${OQS_USE_OPENSSL})
    if(NOT DEFINED OPENSSL_ROOT_DIR)
        if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
            if(EXISTS "/usr/local/opt/openssl@1.1")
                set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1")
            elseif(EXISTS "/opt/homebrew/opt/openssl@1.1")
                set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@1.1")
            endif()
        elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
            set(OPENSSL_ROOT_DIR "/usr")
        endif()
    endif()
    find_package(OpenSSL 1.1.1 REQUIRED)
endif()

set(PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
                   ${PROJECT_SOURCE_DIR}/src/common/common.h
                   ${PROJECT_SOURCE_DIR}/src/common/rand/rand.h
                   ${PROJECT_SOURCE_DIR}/src/common/aes/aes.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha2/sha2.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3x4.h
                   ${PROJECT_SOURCE_DIR}/src/kem/kem.h
                   ${PROJECT_SOURCE_DIR}/src/sig/sig.h)

if(${OQS_ENABLE_KEM_BIKE})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/bike/kem_bike.h)
endif()
if(${OQS_ENABLE_KEM_FRODOKEM})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/frodokem/kem_frodokem.h)
endif()
if(${OQS_ENABLE_KEM_SIKE} OR ${OQS_ENABLE_KEM_SIDH})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/sike/kem_sike.h)
endif()
if(${OQS_ENABLE_SIG_PICNIC})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/picnic/sig_picnic.h)
endif()
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_INCLUDE_HEADERS_START
if(OQS_ENABLE_KEM_CLASSIC_MCELIECE)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/classic_mceliece/kem_classic_mceliece.h)
endif()
if(OQS_ENABLE_KEM_HQC)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/hqc/kem_hqc.h)
endif()
if(OQS_ENABLE_KEM_KYBER)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/kyber/kem_kyber.h)
endif()
if(OQS_ENABLE_KEM_NTRU)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntru/kem_ntru.h)
endif()
if(OQS_ENABLE_KEM_NTRUPRIME)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntruprime/kem_ntruprime.h)
endif()
if(OQS_ENABLE_KEM_SABER)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/saber/kem_saber.h)
endif()
if(OQS_ENABLE_SIG_DILITHIUM)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/dilithium/sig_dilithium.h)
endif()
if(OQS_ENABLE_SIG_FALCON)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/falcon/sig_falcon.h)
endif()
if(OQS_ENABLE_SIG_RAINBOW)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/rainbow/sig_rainbow.h)
endif()
if(OQS_ENABLE_SIG_SPHINCS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/sphincs/sig_sphincs.h)
endif()
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_INCLUDE_HEADERS_END
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
configure_file(src/oqsconfig.h.cmake ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)
set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)

include_directories(${PROJECT_BINARY_DIR}/include)
add_subdirectory(src)

if(NOT ${OQS_BUILD_ONLY_LIB})
    add_subdirectory(tests)

    if (NOT CYGWIN)
      find_package(Doxygen)
      if(DOXYGEN_FOUND)
        set(DOXYFILE ${PROJECT_SOURCE_DIR}/docs/.Doxyfile)
        add_custom_target(
            gen_docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generate API documentation with Doxygen."
            USES_TERMINAL)
      endif()
    endif()

    if(NOT WIN32)
        add_custom_target(
            prettyprint
            COMMAND find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem.*/pqclean_' | grep -v 'sig.*/pqclean_' | xargs astyle --options=.astylerc
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL)
    endif()
endif()
