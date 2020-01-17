find_package(Python3 COMPONENTS Interpreter)

if(USE_OPENSSL)
    include_directories(${OPENSSL_INCLUDE_DIR})
    set(LIBS ${OPENSSL_CRYPTO_LIBRARY})
endif()

if(NOT WIN32)
    set(LIBS ${LIBS} m)
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/git_commit.sh OUTPUT_VARIABLE GIT_COMMIT)
    add_compile_definitions(OQS_COMPILE_GIT_COMMIT="${GIT_COMMIT}")

    # These functions tested here are
    # not part of the liboqs API.
    get_directory_property(AES_IMPL
                           DIRECTORY ${PROJECT_SOURCE_DIR}/src/crypto
                           DEFINITION AES_IMPL)
    add_executable(test_aes test_aes.c ${AES_IMPL} ${PROJECT_SOURCE_DIR}/src/common/common.c)
    target_link_libraries(test_aes ${LIBS})

    get_directory_property(SHA2_IMPL
                           DIRECTORY ${PROJECT_SOURCE_DIR}/src/crypto
                           DEFINITION SHA2_IMPL)
    add_executable(test_hash test_hash.c ${SHA2_IMPL} ${PROJECT_SOURCE_DIR}/src/common/common.c)
    target_link_libraries(test_hash ${LIBS})

    get_directory_property(SHA3_IMPL
                           DIRECTORY ${PROJECT_SOURCE_DIR}/src/crypto
                           DEFINITION SHA3_IMPL)
    add_executable(test_sha3 test_sha3.c ${SHA3_IMPL} ${PROJECT_SOURCE_DIR}/src/common/common.c)
    target_link_libraries(test_sha3 ${LIBS})
endif()

set(LIBS oqs ${LIBS})

# KEM API tests
add_executable(example_kem example_kem.c)
target_link_libraries(example_kem ${LIBS})

add_executable(kat_kem kat_kem.c)
target_link_libraries(kat_kem ${LIBS})

add_executable(test_kem test_kem.c)
target_link_libraries(test_kem ${LIBS})

add_executable(speed_kem speed_kem.c)
target_link_libraries(speed_kem ${LIBS})

# SIG API tests
add_executable(example_sig example_sig.c)
target_link_libraries(example_sig ${LIBS})

add_executable(kat_sig kat_sig.c)
target_link_libraries(kat_sig ${LIBS})

add_executable(test_sig test_sig.c)
target_link_libraries(test_sig ${LIBS})

add_executable(speed_sig speed_sig.c)
target_link_libraries(speed_sig ${LIBS})

add_custom_target(
    run_tests
    COMMAND ${Python3_EXECUTABLE} -m pytest --verbose --junitxml=${CMAKE_BINARY_DIR}/test-results.xml --build_tool=${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS oqs kat_kem kat_sig test_kem test_sig
    USES_TERMINAL)
