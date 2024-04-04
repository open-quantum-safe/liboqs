# SPDX-License-Identifier: MIT

option(OQS_ENABLE_TEST_CONSTANT_TIME "Build test suite with support for Valgrind-based detection of non-constant time behaviour." OFF)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wbad-function-cast)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wnarrowing)
    add_compile_options(-Wconversion)
endif()
if (MINGW OR MSYS OR CYGWIN)
    add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
    add_compile_options(-Wno-unknown-pragmas)
    add_compile_options(-Wno-unused-parameter)
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
    # ignore warning for test apps
    add_compile_options(/wd4996 /wd4244)
endif()

if(OQS_USE_OPENSSL)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

if(NOT WIN32)
    set(LIBM m)
endif()

# List oqs-internal after oqs so that oqs has linking precedence.
set(TEST_DEPS oqs oqs-internal ${LIBM})
if(OQS_USE_PTHREADS)
    set(TEST_DEPS ${TEST_DEPS} Threads::Threads)
endif()

if(NOT WIN32)
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/git_commit.sh OUTPUT_VARIABLE GIT_COMMIT)
    add_definitions(-DOQS_COMPILE_GIT_COMMIT="${GIT_COMMIT}")

    add_executable(test_aes test_aes.c)
    target_link_libraries(test_aes PRIVATE ${TEST_DEPS})

    add_executable(test_hash test_hash.c)
    target_link_libraries(test_hash PRIVATE ${TEST_DEPS})

    add_executable(test_sha3 test_sha3.c)
    target_link_libraries(test_sha3 PRIVATE ${TEST_DEPS})

    add_executable(speed_common speed_common.c)
    target_link_libraries(speed_common PRIVATE ${TEST_DEPS})

    set(UNIX_TESTS test_aes test_hash test_sha3 speed_common)

    set(PYTHON3_EXEC python3)
else()
    set(PYTHON3_EXEC python)
endif()

# KEM API tests
add_executable(example_kem example_kem.c)
target_link_libraries(example_kem PRIVATE ${TEST_DEPS})

add_executable(kat_kem kat_kem.c test_helpers.c)
target_link_libraries(kat_kem PRIVATE ${TEST_DEPS})
if(CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Windows" AND BUILD_SHARED_LIBS)
    # workaround for Windows .dll cross-compiling
    target_link_options(kat_kem PRIVATE -Wl,--allow-multiple-definition)
endif()

add_executable(test_kem test_kem.c)
target_link_libraries(test_kem PRIVATE ${TEST_DEPS})

add_executable(test_kem_mem test_kem_mem.c)
target_link_libraries(test_kem_mem PRIVATE ${TEST_DEPS})

add_executable(speed_kem speed_kem.c)
target_link_libraries(speed_kem PRIVATE ${TEST_DEPS})

set(KEM_TESTS example_kem kat_kem test_kem test_kem_mem speed_kem vectors_kem)

# SIG API tests
add_executable(example_sig example_sig.c)
target_link_libraries(example_sig PRIVATE ${TEST_DEPS})

add_executable(kat_sig kat_sig.c test_helpers.c)
target_link_libraries(kat_sig PRIVATE ${TEST_DEPS})
if(CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Windows" AND BUILD_SHARED_LIBS)
    # workaround for Windows .dll cross-compiling
    target_link_options(kat_sig PRIVATE -Wl,--allow-multiple-definition)
endif()

add_executable(test_sig test_sig.c)
target_link_libraries(test_sig PRIVATE ${TEST_DEPS})

add_executable(test_sig_mem test_sig_mem.c)
target_link_libraries(test_sig_mem PRIVATE ${TEST_DEPS})

add_executable(speed_sig speed_sig.c)
target_link_libraries(speed_sig PRIVATE ${TEST_DEPS})

set(SIG_TESTS example_sig kat_sig test_sig test_sig_mem speed_sig vectors_sig)

add_executable(dump_alg_info dump_alg_info.c)
target_link_libraries(dump_alg_info PRIVATE ${TEST_DEPS})

# Intermediate values vector tests
add_executable(vectors_sig vectors_sig.c)
target_link_libraries(vectors_sig PRIVATE ${TEST_DEPS})

add_executable(vectors_kem vectors_kem.c)
target_link_libraries(vectors_kem PRIVATE ${TEST_DEPS})

# Enable Valgrind-based timing side-channel analysis for test_kem and test_sig
if(OQS_ENABLE_TEST_CONSTANT_TIME AND NOT OQS_DEBUG_BUILD)
    message(WARNING "OQS_ENABLE_TEST_CONSTANT_TIME is incompatible with CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}.")
    set(OQS_ENABLE_TEST_CONSTANT_TIME OFF)
endif()

# Record compile options -- from target speed_kem - don't set any options only for speed_kem!
get_property(OQS_COMPILE_OPTIONS TARGET speed_kem PROPERTY COMPILE_OPTIONS)
add_definitions(-DOQS_COMPILE_OPTIONS="[${OQS_COMPILE_OPTIONS}]")

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # With Visual studio the output of tests go into a folder with the configuration option. Force it to the same folder as if
    # generating with Ninja
    set_target_properties(
        dump_alg_info ${KEM_TESTS} ${SIG_TESTS}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_BINARY_DIR}/tests"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_BINARY_DIR}/tests"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/tests"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_BINARY_DIR}/tests")
endif()

# TODO: Get CMake to find python.
# and set PATH variable in Windows
# for DLL builds.
add_custom_target(
    run_tests
    # skip long KAT tests
    COMMAND ${CMAKE_COMMAND} -E env OQS_BUILD_DIR=${CMAKE_BINARY_DIR} ${PYTHON3_EXEC} -m pytest --verbose --numprocesses=auto --ignore=scripts/copy_from_upstream/repos --ignore=tests/test_kat_all.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS oqs dump_alg_info ${KEM_TESTS} ${SIG_TESTS} ${UNIX_TESTS}
    USES_TERMINAL)
