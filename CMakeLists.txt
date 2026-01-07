# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.15)
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
if(POLICY CMP0066)
    cmake_policy(SET CMP0066 NEW)
endif()
if(POLICY CMP0067)
    cmake_policy(SET CMP0067 NEW)
endif()

project(liboqs C ASM)

option(OQS_DIST_BUILD "Build distributable library with optimized code for several CPU microarchitectures. Enables run-time CPU feature detection." ON)
option(OQS_BUILD_ONLY_LIB "Build only liboqs and do not expose build targets for tests, documentation, and pretty-printing available." OFF)
set(OQS_MINIMAL_BUILD "" CACHE STRING "Only build specifically listed algorithms.")
option(OQS_LIBJADE_BUILD "Enable formally verified implementation of supported algorithms from libjade." OFF)
option(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE "Permit compilation on an an unsupported architecture." OFF)
option(OQS_STRICT_WARNINGS "Enable all compiler warnings." OFF)
option(OQS_EMBEDDED_BUILD "Compile liboqs for an Embedded environment without a full standard library." OFF)
option(OQS_USE_CUPQC "Utilize cuPQC as the backend for supported PQC algorithms." OFF)
option(OQS_USE_ICICLE "Utilize ICICLE as the backend for supported PQC algorithms." OFF)

# Libfuzzer isn't supported on gcc
if('${CMAKE_C_COMPILER_ID}' STREQUAL 'Clang')
    option(OQS_BUILD_FUZZ_TESTS "Build fuzz test suite" OFF)
endif()


set(OQS_OPT_TARGET auto CACHE STRING "The target microarchitecture for optimization.")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(OQS_VERSION_MAJOR 0)
set(OQS_VERSION_MINOR 15)
set(OQS_VERSION_PATCH 0)
set(OQS_VERSION_PRE_RELEASE "")
set(OQS_VERSION_TEXT "${OQS_VERSION_MAJOR}.${OQS_VERSION_MINOR}.${OQS_VERSION_PATCH}${OQS_VERSION_PRE_RELEASE}")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")
set(OQS_MINIMAL_GCC_VERSION "7.1.0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Determine the flags for fuzzing. Use OSS-Fuzz's configuration if available, otherwise fall back to defaults.
if(DEFINED ENV{LIB_FUZZING_ENGINE})
    set(FUZZING_ENGINE $ENV{LIB_FUZZING_ENGINE})
    set(FUZZING_COMPILE_FLAGS "")
    set(FUZZING_LINK_FLAGS "${FUZZING_ENGINE}")
else()
    set(FUZZING_COMPILE_FLAGS "-fsanitize=fuzzer,address")
    set(FUZZING_LINK_FLAGS "-fsanitize=fuzzer,address")
endif()

# heuristic check to see whether we're running on a RaspberryPi
if(EXISTS "/opt/vc/include/bcm_host.h")
	add_definitions( -DOQS_USE_RASPBERRY_PI )
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    set(ARCH "x86_64")
    set(ARCH_X86_64 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_X86_64_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i586|i686")
    set(ARCH "i586")
    set(ARCH_X86 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_X86_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm64v8")
    set(ARCH "arm64v8")
    set(ARCH_ARM64v8 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_ARM64_V8_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armel|armhf|armv7|arm32v7")
    set(ARCH "arm32v7")
    set(ARCH_ARM32v7 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_ARM32_V7_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ppc64le|powerpc64le")
    set(ARCH "ppc64le")
    set(ARCH_PPC64LE ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_PPC64LE_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(ppc64|powerpc64)")
    message(WARNING "There is currently no CI for: " ${CMAKE_SYSTEM_PROCESSOR})
    set(ARCH "ppc64")
    set(ARCH_PPC64 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_PPC64_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(ppc|powerpc)")
    message(WARNING "There is currently no CI for: " ${CMAKE_SYSTEM_PROCESSOR})
    # CMake uses uname to derive CMAKE_SYSTEM_PROCESSOR value, so on Darwin
    # the value is identical for ppc and ppc64. To have the right build arch
    # in 64-bit case, we use CMAKE_OSX_ARCHITECTURES.
    if(APPLE AND CMAKE_OSX_ARCHITECTURES STREQUAL "ppc64")
        set(ARCH "ppc64")
        set(ARCH_PPC64 ON)
        if(${OQS_DIST_BUILD})
            set(OQS_DIST_PPC64_BUILD ON)
        endif()
    else()
        set(ARCH "ppc")
        set(ARCH_PPC ON)
        if(${OQS_DIST_BUILD})
            set(OQS_DIST_PPC_BUILD ON)
        endif()
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "s390x")
    set(ARCH "s390x")
    set(ARCH_S390X ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_S390X_BUILD ON)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv")
    set(ARCH "riscv")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch64")
    set(ARCH "loongarch64")
    set(ARCH_LOONGARCH64 ON)
    if(${OQS_DIST_BUILD})
        set(OQS_DIST_LOONGARCH64_BUILD ON)
    endif()
elseif(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE)
    message(WARNING "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR})
    message(WARNING "Compilation on an unsupported processor should only be used for testing, as it may result an insecure configuration, for example due to variable-time instructions leaking secret information.")
else()
    message(FATAL_ERROR "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR} ". Override by setting OQS_PERMIT_UNSUPPORTED_ARCHITECTURE=ON")
endif()

if(${OQS_USE_CUPQC})
    # CMAKE's CUDA language requires CMAKE 3.18
    cmake_minimum_required (VERSION 3.18)
    enable_language(CUDA)
    if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
      set(CMAKE_CUDA_ARCHITECTURES 80 90)
    endif()
    find_package(cuPQC 0.2.0 REQUIRED)
endif()

if(OQS_USE_ICICLE)
    enable_language(CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_package(icicle_pqc_package REQUIRED)
endif()


if (NOT ((CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin") AND (ARCH_X86_64 STREQUAL "ON")) AND (OQS_LIBJADE_BUILD STREQUAL "ON"))
    message(FATAL_ERROR "Building liboqs with libjade implementations from libjade is only supported on Linux and Darwin on x86_64.")
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

if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
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
        endif()
    endif()
    find_package(OpenSSL 1.1.1 REQUIRED)

    if(OQS_DLOPEN_OPENSSL)
      find_program(OBJDUMP objdump)
      if(NOT OBJDUMP)
	message(FATAL_ERROR "objdump not found. Please install it from binutils.")
      endif()
      execute_process(
	COMMAND ${OBJDUMP} -p ${OPENSSL_CRYPTO_LIBRARY}
	COMMAND sed -n "s/[ 	]\\{1,\\}SONAME[ 	]\\{1,\\}//p"
	OUTPUT_VARIABLE OQS_OPENSSL_CRYPTO_SONAME
	OUTPUT_STRIP_TRAILING_WHITESPACE
	COMMAND_ERROR_IS_FATAL ANY)
      message(STATUS "OpenSSL dlopen SONAME: " ${OQS_OPENSSL_CRYPTO_SONAME})
    endif()
endif()

set(PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
                   ${PROJECT_SOURCE_DIR}/src/common/aes/aes_ops.h
                   ${PROJECT_SOURCE_DIR}/src/common/common.h
                   ${PROJECT_SOURCE_DIR}/src/common/rand/rand.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha2/sha2_ops.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3_ops.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3x4_ops.h
                   ${PROJECT_SOURCE_DIR}/src/kem/kem.h
                   ${PROJECT_SOURCE_DIR}/src/sig/sig.h
                   ${PROJECT_SOURCE_DIR}/src/sig_stfl/sig_stfl.h)

set(INTERNAL_HEADERS ${PROJECT_SOURCE_DIR}/src/common/aes/aes.h
                     ${PROJECT_SOURCE_DIR}/src/common/rand/rand_nist.h
                     ${PROJECT_SOURCE_DIR}/src/common/sha2/sha2.h
                     ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3.h
                     ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3x4.h)

if(${OQS_ENABLE_KEM_BIKE})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/bike/kem_bike.h)
endif()
if(${OQS_ENABLE_KEM_FRODOKEM})
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/frodokem/kem_frodokem.h)
endif()
if(OQS_ENABLE_KEM_NTRUPRIME)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntruprime/kem_ntruprime.h)
endif()
if(OQS_ENABLE_KEM_NTRU)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntru/kem_ntru.h)
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
if(OQS_ENABLE_KEM_ML_KEM)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ml_kem/kem_ml_kem.h)
endif()
if(OQS_ENABLE_SIG_ML_DSA)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/ml_dsa/sig_ml_dsa.h)
endif()
if(OQS_ENABLE_SIG_FALCON)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/falcon/sig_falcon.h)
endif()
if(OQS_ENABLE_SIG_MAYO)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/mayo/sig_mayo.h)
endif()
if(OQS_ENABLE_SIG_CROSS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/cross/sig_cross.h)
endif()
if(OQS_ENABLE_SIG_UOV)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/uov/sig_uov.h)
endif()
if(OQS_ENABLE_SIG_SNOVA)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/snova/sig_snova.h)
endif()
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_INCLUDE_HEADERS_END
if(OQS_ENABLE_SIG_SLH_DSA)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/slh_dsa/sig_slh_dsa.h)
endif()
if(OQS_ENABLE_SIG_STFL_XMSS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig_stfl/xmss/sig_stfl_xmss.h)
endif()
if(OQS_ENABLE_SIG_STFL_LMS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig_stfl/lms/sig_stfl_lms.h)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${INTERNAL_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
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
            COMMAND ${PROJECT_SOURCE_DIR}/scripts/run_doxygen.sh ${DOXYGEN_EXECUTABLE} ${DOXYFILE} ${PROJECT_BINARY_DIR}
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
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "www.openquantumsafe.org")
set(CPACK_PACKAGE_VERSION ${OQS_VERSION_TEXT})
if(${OQS_USE_OPENSSL})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, openssl")
else()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "www.openquantumsafe.org")
include(CPack)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.CMake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

