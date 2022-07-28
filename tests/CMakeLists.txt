# SPDX-License-Identifier: MIT

option(OQS_ENABLE_TEST_CONSTANT_TIME "Build test suite with support for Valgrind-based detection of non-constant time behaviour." OFF)

list(APPEND OQS_TESTS_C_FLAGS "")
list(APPEND OQS_TESTS_C_DEFINITIONS "")
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    list(APPEND OQS_TESTS_C_FLAGS "-Wbad-function-cast")
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    list(APPEND OQS_TESTS_C_FLAGS
      "-Wcast-qual"
      "-Wnarrowing"
      "-Wconversion"
    )
endif()
if (MINGW OR MSYS OR CYGWIN)
    list(APPEND OQS_TESTS_C_DEFINITIONS "__USE_MINGW_ANSI_STDIO=1")
    list(APPEND OQS_TESTS_C_FLAGS
      "-Wno-unknown-pragmas"
      "-Wno-unused-parameter"
    )
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
    # ignore warning for test apps
    list(APPEND OQS_TESTS_C_FLAGS
      "/wd4996"
      "/wd4244"
    )
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)
get_target_property(PYTHON3EXEC Python3::Interpreter LOCATION)
list(APPEND OQS_PYTEST_ARGS "-m" "pytest" "--verbose" "--ignore=scripts/copy_from_upstream/repos")
if (NOT APPLE)
  # '--numprocesses=auto does not seem to be supported on Darwin
  list(APPEND OQS_PYTEST_ARGS "--numprocesses=auto")
endif()
add_custom_target(
    run_tests
    COMMAND ${CMAKE_COMMAND} -E env OQS_BUILD_DIR=${CMAKE_BINARY_DIR} ${PYTHON3EXEC} ${OQS_PYTEST_ARGS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    USES_TERMINAL)
#! Add a test
#
# \arg:NAME Name of the test
# \arg:UNIX_ONLY test for unix platforms only (not WIN32)
# \group:sources Sources of the test
function(oqs_add_test)
  cmake_parse_arguments(ADD_TEST "UNIX_ONLY" "NAME" "SOURCES;LIBRARIES" ${ARGN})

  if(ADD_TEST_UNIX_ONLY AND WIN32)
    return()
  endif()

  if (NOT DEFINED ADD_TEST_NAME)
    message(FATAL_ERROR "`NAME` is not defined")
  endif()
  set(target "${ADD_TEST_NAME}")
  add_executable("${target}" ${ADD_TEST_SOURCES})

  set_target_properties(${target}
    PROPERTIES
    # Compiler, standard and code properties
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET "hidden"
    EXPORT_COMPILE_COMMANDS ON
    # Build properties
    VERSION ${OQS_VERSION_TEXT}
  )

  get_target_property(OQS_C_FLAGS oqs COMPILE_OPTIONS)
  oqs_add_c_flags(${target} REQUIRED PRIVATE ${OQS_C_FLAGS})

  oqs_add_c_flags(${target} REQUIRED PRIVATE ${OQS_TESTS_C_FLAGS})
  target_compile_definitions(${target} PRIVATE ${OQS_TESTS_C_DEFINITIONS})

  if(OQS_USE_OPENSSL)
    target_include_directories(${target} SYSTEM PRIVATE "${OPENSSL_INCLUDE_DIR}")
  endif()

  if(ADD_TEST_UNIX_ONLY)

    if(NOT DEFINED GIT_COMMIT)
      execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/git_commit.sh OUTPUT_VARIABLE commit)
      set(GIT_COMMIT "${commit}" PARENT_SCOPE)
    else()
      set(commit "${GIT_COMMIT}")
    endif()
    target_compile_definitions(${target} PRIVATE "DOQS_COMPILE_GIT_COMMIT=${commit}")

    set(unix_tests "${UNIX_TESTS}")
    list(APPEND unix_tests ${target})
    set(UNIX_TESTS "${unix_tests}" PARENT_SCOPE)
  endif()
  if(ADD_TEST_LIBRARIES)
    target_link_libraries(${target} PRIVATE ${ADD_TEST_LIBRARIES})
  endif()
  if(NOT WIN32)
    target_link_libraries(${target} PRIVATE m)
  endif()

  add_test(NAME test_${target}
    COMMAND ${target}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
  set_tests_properties(test_${target} PROPERTIES
    ENVIRONMENT "OQS_BUILD_DIR=${CMAKE_BINARY_DIR}")
  add_dependencies(run_tests ${target})
endfunction()

if(OQS_USE_OPENSSL)
  set(INTERNAL_TEST_DEPS ${OPENSSL_CRYPTO_LIBRARY})
else()
  set(INTERNAL_TEST_DEPS "")
endif()

get_target_property(sanitizers oqs SANITIZERS)
if(sanitizers)
  list(APPEND INTERNAL_TEST_DEPS ${sanitizers})
endif()

set(LIBM m)
oqs_add_test(NAME test_aes
  UNIX_ONLY
  SOURCES test_aes.c ${COMMON_OBJS}
  LIBRARIES ${INTERNAL_TEST_DEPS}
)
oqs_add_test(NAME test_hash
  UNIX_ONLY
  SOURCES test_hash.c ${COMMON_OBJS}
  LIBRARIES ${INTERNAL_TEST_DEPS}
)
oqs_add_test(NAME test_sha3
  UNIX_ONLY
  SOURCES test_sha3.c ${COMMON_OBJS}
  LIBRARIES ${INTERNAL_TEST_DEPS}
)
oqs_add_test(NAME speed_common
  UNIX_ONLY
  SOURCES speed_common.c ${COMMON_OBJS}
  LIBRARIES ${INTERNAL_TEST_DEPS}
)

# KEM tests
oqs_add_test(NAME example_kem
  SOURCES example_kem.c
  LIBRARIES oqs
)
oqs_add_test(NAME kat_kem
  SOURCES kat_kem.c
  LIBRARIES oqs
)
oqs_add_test(NAME test_kem
  SOURCES test_kem.c
  LIBRARIES oqs
)
if((CMAKE_C_COMPILER_ID MATCHES "Clang") OR (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
    target_link_libraries(test_kem PRIVATE Threads::Threads)
endif()
oqs_add_test(NAME test_kem_mem
  SOURCES test_kem_mem.c
  LIBRARIES oqs
)
oqs_add_test(NAME speed_kem
  SOURCES speed_kem.c
  LIBRARIES oqs
)

# SIG API tests
oqs_add_test(NAME example_sig
  SOURCES example_sig.c
  LIBRARIES oqs
)
oqs_add_test(NAME kat_sig
  SOURCES kat_sig.c
  LIBRARIES oqs
)
oqs_add_test(NAME test_sig
  SOURCES test_sig.c
  LIBRARIES oqs
)
if((CMAKE_C_COMPILER_ID MATCHES "Clang") OR (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
    target_link_libraries(test_sig PRIVATE Threads::Threads)
endif()
oqs_add_test(NAME test_sig_mem
  SOURCES test_sig_mem.c
  LIBRARIES oqs
)
oqs_add_test(NAME speed_sig
  SOURCES speed_sig.c
  LIBRARIES oqs
)
oqs_add_test(NAME dump_alg_info
  SOURCES dump_alg_info.c
  LIBRARIES oqs
)
# Enable Valgrind-based timing side-channel analysis for test_kem and test_sig
if(OQS_ENABLE_TEST_CONSTANT_TIME AND NOT OQS_DEBUG_BUILD)
    message(WARNING "OQS_ENABLE_TEST_CONSTANT_TIME is incompatible with CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}.")
    set(OQS_ENABLE_TEST_CONSTANT_TIME OFF)
endif()
# Record compile options -- from target speed_kem - don't set any options only for speed_kem!
get_property(OQS_COMPILE_OPTIONS TARGET speed_kem PROPERTY COMPILE_OPTIONS)
target_compile_definitions(speed_kem PRIVATE "DOQS_COMPILE_OPTIONS=[${OQS_COMPILE_OPTIONS}]")

