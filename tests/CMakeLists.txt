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

if(NOT WIN32)
    set(LIBM m)

    if(OQS_USE_OPENSSL)
        include_directories(${OPENSSL_INCLUDE_DIR})
        set(INTERNAL_TEST_DEPS ${OPENSSL_CRYPTO_LIBRARY} ${LIBM})
    else()
        set(INTERNAL_TEST_DEPS ${LIBM})
    endif()
    if(DEFINED SANITIZER_LD_FLAGS)
        set(INTERNAL_TEST_DEPS "${INTERNAL_TEST_DEPS} ${SANITIZER_LD_FLAGS}")
    endif()
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/git_commit.sh OUTPUT_VARIABLE GIT_COMMIT)
    add_definitions(-DOQS_COMPILE_GIT_COMMIT="${GIT_COMMIT}")

    add_executable(test_aes test_aes.c $<TARGET_OBJECTS:common>)
    target_link_libraries(test_aes PRIVATE ${INTERNAL_TEST_DEPS})

    add_executable(test_hash test_hash.c $<TARGET_OBJECTS:common>)
    target_link_libraries(test_hash PRIVATE ${INTERNAL_TEST_DEPS})

    add_executable(test_sha3 test_sha3.c $<TARGET_OBJECTS:common>)
    target_link_libraries(test_sha3 PRIVATE ${INTERNAL_TEST_DEPS})

    set(UNIX_TESTS test_aes test_hash test_sha3)

    set(PYTHON3_EXEC python3)
    if(BUILD_SHARED_LIBS)
        set(USE_LIBOQS_SO --use_liboqs_so)
    endif()
else()
    set(PYTHON3_EXEC python)
endif()

set(API_TEST_DEPS oqs ${LIBM})

# KEM API tests
add_executable(example_kem example_kem.c)
target_link_libraries(example_kem PRIVATE ${API_TEST_DEPS})

add_executable(kat_kem kat_kem.c)
target_link_libraries(kat_kem PRIVATE ${API_TEST_DEPS})

add_executable(test_kem test_kem.c)
if((CMAKE_C_COMPILER_ID MATCHES "Clang") OR (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
    target_link_libraries(test_kem PRIVATE ${API_TEST_DEPS} Threads::Threads)
else ()
    target_link_libraries(test_kem PRIVATE ${API_TEST_DEPS})
endif()

add_executable(speed_kem speed_kem.c)
target_link_libraries(speed_kem PRIVATE ${API_TEST_DEPS})

# SIG API tests
add_executable(example_sig example_sig.c)
target_link_libraries(example_sig PRIVATE ${API_TEST_DEPS})

add_executable(kat_sig kat_sig.c)
target_link_libraries(kat_sig PRIVATE ${API_TEST_DEPS})

add_executable(test_sig test_sig.c)
if((CMAKE_C_COMPILER_ID MATCHES "Clang") OR (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
    target_link_libraries(test_sig PRIVATE ${API_TEST_DEPS} Threads::Threads)
else ()
    target_link_libraries(test_sig PRIVATE ${API_TEST_DEPS})
endif()

add_executable(speed_sig speed_sig.c)
target_link_libraries(speed_sig PRIVATE ${API_TEST_DEPS})

# TODO: Get CMake to find python.
# and set PATH variable in Windows
# for DLL builds.
add_custom_target(
    run_tests
    COMMAND ${PYTHON3_EXEC} -m pytest --verbose --numprocesses=auto --build_tool=${CMAKE_MAKE_PROGRAM} ${USE_LIBOQS_SO} --junitxml=${CMAKE_BINARY_DIR}/test-results/pytest/test-results.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS oqs example_kem kat_kem test_kem example_sig kat_sig test_sig ${UNIX_TESTS}
    USES_TERMINAL)
