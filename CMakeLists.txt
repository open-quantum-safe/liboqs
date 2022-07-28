# SPDX-License-Identifier: MIT
cmake_minimum_required (VERSION 3.5 FATAL_ERROR)
enable_testing()
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
# Allow /src/CMakeLists.txt to add link library to oqs defined in /CMakeLists.txt
if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()


project(liboqs VERSION 0.7.2 LANGUAGES C ASM)

add_library(oqs)

option(OQS_DIST_BUILD "Build distributable library with optimized code for several CPU microarchitectures. Enables run-time CPU feature detection." OFF)
option(OQS_BUILD_ONLY_LIB "Build only liboqs and do not expose build targets for tests, documentation, and pretty-printing available." OFF)
set(OQS_MINIMAL_BUILD "" CACHE STRING "Only build specifically listed algorithms.")
option(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE "Permit compilation on an an unsupported architecture." OFF)

set(OQS_OPT_TARGET auto CACHE STRING "The target microarchitecture for optimization.")

set(OQS_VERSION_TEXT "0.7.2-dev")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")

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

set_property(GLOBAL PROPERTY OQS_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
                   ${PROJECT_SOURCE_DIR}/src/common/common.h
                   ${PROJECT_SOURCE_DIR}/src/common/rand/rand.h
                   ${PROJECT_SOURCE_DIR}/src/common/aes/aes.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha2/sha2.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3x4.h
                   ${PROJECT_SOURCE_DIR}/src/kem/kem.h
                   ${PROJECT_SOURCE_DIR}/src/sig/sig.h)

#! enable_alg : Enable algorithm depending on an option
#
# Enable an algorithm, if the option OQS_ENABLE_<TYPE>_<NAME> evaluates
# to true
#
# This function finds the header of the algorithm using its type and its name:
#  ${PROJECT_SOURCE_DIR}/src/<TYPE>/<NAME>/<TYPE>_<NAME>.h
# Example: for the kem 'HQC':
#  ${PROJECT_SOURCE_DIR}/src/kem/hqc/kem_hqc.h
#
# \arg:TYPE Type of the algorithm: 'KEM' or 'SIG'
# \group:NAMES List of names matching the algorithm
# \group:HEADERS List of headers to add to the list of public headers
function(enable_alg)
  list(APPEND supported_types "KEM" "SIG")
  cmake_parse_arguments(ENABLE_ALG "" "TYPE" "NAMES;HEADERS" ${ARGN})

  if(NOT DEFINED ENABLE_ALG_TYPE)
    message(FATAL_ERROR "`TYPE` is not defined")
  endif()
  list(FIND supported_types "${ENABLE_ALG_TYPE}" index)
  if(index EQUAL -1)
    message(FATAL_ERROR "Type '${ENABLE_ALG_TYPE}' is not supported")
  endif()

  if(NOT DEFINED ENABLE_ALG_NAMES)
    message(FATAL_ERROR "`NAMES` is not defined")
  endif()
  list(LENGTH ENABLE_ALG_NAMES names_count)
  if(names_count EQUAL 0)
    message(FATAL_ERROR "At least one name is required")
  endif()

  list(LENGTH ENABLE_ALG_NAMES headers_count)
  if(headers_count EQUAL 0)
    message(FATAL_ERROR "At least one header file is required for ${tuple}")
  endif()

  foreach(name ${ENABLE_ALG_NAMES})
    set(tuple "${ENABLE_ALG_TYPE}_${name}")
    if (OQS_ENABLE_${tuple})
      foreach(header ${ENABLE_ALG_HEADERS})
        if(NOT EXISTS ${header})
          message(FATAL_ERROR "Cannot enable '${name}': ${header} does not exist")
        endif()
      endforeach()
      message(STATUS "${ENABLE_ALG_TYPE} ${name} is enabled")
      get_property(tmp GLOBAL PROPERTY OQS_PUBLIC_HEADERS)
      list(APPEND tmp ${ENABLE_ALG_HEADERS})
      set_property(GLOBAL PROPERTY OQS_PUBLIC_HEADERS ${tmp})
      return()
    else()
      message(STATUS "${ENABLE_ALG_TYPE} ${name} is disabled")
    endif()
  endforeach()
endfunction()

#! enable_algs : Enable algorithms depending on an option
#
# Enable several algorithms, if the option OQS_ENABLE_<TYPE>_<NAME> evaluates
# to true
#
# This function finds the header of the algorithm using its type and its name:
#  ${PROJECT_SOURCE_DIR}/src/<TYPE>/<NAME>/<TYPE>_<NAME>.h
# Example: for the kem 'HQC':
#  ${PROJECT_SOURCE_DIR}/src/kem/hqc/kem_hqc.h
#
# \arg:TYPE Type of the algorithm: 'KEM' or 'SIG'
# \group:NAMES List of names
function(enable_algs)
  cmake_parse_arguments(ENABLE_ALGS "" "TYPE" "NAMES" ${ARGN})
  string(TOLOWER "${ENABLE_ALGS_TYPE}" type)
  foreach(nameU ${ENABLE_ALGS_NAMES})
    string(TOLOWER "${nameU}" name)
    set(header "${PROJECT_SOURCE_DIR}/src/${type}/${name}/${type}_${name}.h")
    enable_alg(TYPE ${ENABLE_ALGS_TYPE} NAMES ${nameU} HEADERS ${header})
  endforeach()
endfunction()

#! enable_kems : Enable KEM algorithms
#
# Enable several KEM algorithms, if the option OQS_ENABLE_KEM_<NAME> evaluates
# to true
#
# \argn:List of names
function(enable_kems)
  enable_algs(TYPE KEM ${ARGN})
endfunction()

#! enable_sigs : Enable SIGS algorithms
#
# Enable several SIG algorithms, if the option OQS_ENABLE_SIG_<NAME> evaluates
# to true
#
# \argn:List of names
function(enable_sigs)
  enable_algs(TYPE SIG ${ARGN})
endfunction()

if(OQS_ENABLE_KEM_SIDH)
  set(OQS_ENABLE_KEM_SIKE)
endif()

enable_kems(NAMES
  "BIKE" "FRODOKEM" "SIKE" "CLASSIC_MCELIECE" "HQC" "KYBER" "NTRU" "NTRUPRIME" "SABER")

enable_sigs(NAMES
  "PICNIC" "FALCON" "RAINBOW" "DILITHIUM" "SPHINCS"
)
get_property(PUBLIC_HEADERS GLOBAL PROPERTY OQS_PUBLIC_HEADERS)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
configure_file(src/oqsconfig.h.cmake ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)
list(APPEND PUBLIC_HEADERS ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)

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
            COMMAND ${PROJECT_SOURCE_DIR}/scripts/run_doxygen.sh ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
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
