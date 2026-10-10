# SPDX-License-Identifier: MIT

add_executable(fuzz_test_sig fuzz_test_sig.c)
target_link_libraries(fuzz_test_sig PRIVATE ${TEST_DEPS})
set_target_properties(fuzz_test_sig PROPERTIES
    COMPILE_FLAGS "${FUZZING_COMPILE_FLAGS}"
    LINK_FLAGS "${FUZZING_LINK_FLAGS}"
)
add_executable(fuzz_test_kem fuzz_test_kem.c)
target_link_libraries(fuzz_test_kem PRIVATE ${TEST_DEPS})
set_target_properties(fuzz_test_kem PROPERTIES
    COMPILE_FLAGS "${FUZZING_COMPILE_FLAGS}"
    LINK_FLAGS "${FUZZING_LINK_FLAGS}"
)
add_executable(fuzz_test_sig_stfl_xmss fuzz_test_sig_stfl_xmss.c)
target_link_libraries(fuzz_test_sig_stfl_xmss PRIVATE ${TEST_DEPS})
set_target_properties(fuzz_test_sig_stfl_xmss PROPERTIES
    COMPILE_FLAGS "${FUZZING_COMPILE_FLAGS}"
    LINK_FLAGS "${FUZZING_LINK_FLAGS}"
)
add_executable(fuzz_test_sig_stfl_lms fuzz_test_sig_stfl_lms.c)
target_link_libraries(fuzz_test_sig_stfl_lms PRIVATE ${TEST_DEPS})
set_target_properties(fuzz_test_sig_stfl_lms PROPERTIES
    COMPILE_FLAGS "${FUZZING_COMPILE_FLAGS}"
    LINK_FLAGS "${FUZZING_LINK_FLAGS}"
)

