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

add_executable(test_kem test_kem.c)
target_link_libraries(test_kem PRIVATE ${TEST_DEPS})

add_executable(test_kem_mem test_kem_mem.c)
target_link_libraries(test_kem_mem PRIVATE ${TEST_DEPS})

add_executable(speed_kem speed_kem.c)
target_link_libraries(speed_kem PRIVATE ${TEST_DEPS})

set(KEM_TESTS example_kem kat_kem test_kem test_kem_mem speed_kem)

# SIG API tests
add_executable(example_sig example_sig.c)
target_link_libraries(example_sig PRIVATE ${TEST_DEPS})

add_executable(kat_sig kat_sig.c test_helpers.c)
target_link_libraries(kat_sig PRIVATE ${TEST_DEPS})

add_executable(test_sig test_sig.c)
target_link_libraries(test_sig PRIVATE ${TEST_DEPS})

add_executable(test_sig_mem test_sig_mem.c)
target_link_libraries(test_sig_mem PRIVATE ${TEST_DEPS})

add_executable(speed_sig speed_sig.c)
target_link_libraries(speed_sig PRIVATE ${TEST_DEPS})

set(SIG_TESTS example_sig kat_sig test_sig test_sig_mem speed_sig)

add_executable(dump_alg_info dump_alg_info.c)
target_link_libraries(dump_alg_info PRIVATE ${TEST_DEPS})

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

add_test(
	NAME test_alg_info
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_alg_info.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_binary
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_binary.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_cmdline
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_cmdline.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_code_conventions
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_code_conventions.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_constant_time
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_constant_time.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_distbuild
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_distbuild.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_hash
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_hash.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_kat
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_kat.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# test_kat_all takes a very long time. Disable by default.
if (OQS_ENABLE_LONG_TESTS)
	add_test(
		NAME test_kat_all
		COMMAND ${PYTHON3_EXEC}
		-m pytest 
		tests/test_kat_all.py
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	set_tests_properties(test_kat_all
		PROPERTIES ENVIRONMENT "OQS_BUILD_DIR=${CMAKE_BINARY_DIR}"
	)
endif()
add_test(
	NAME test_leaks
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_leaks.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_mem
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_mem.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_test(
	NAME test_speed
	COMMAND ${PYTHON3_EXEC}
	-m pytest 
	tests/test_speed.py
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(
	test_alg_info
	test_binary
	test_cmdline
	test_code_conventions
	test_constant_time
	test_distbuild
	test_hash
	test_kat
	test_speed
	test_leaks
	test_mem
	test_speed
	PROPERTIES ENVIRONMENT "OQS_BUILD_DIR=${CMAKE_BINARY_DIR}"
)
