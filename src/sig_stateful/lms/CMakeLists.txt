# SPDX-License-Identifier:
set(SRCS
         external/endian.c
         external/hash.c
         external/hss.c
         external/hss_alloc.c
         external/hss_aux.c
         external/hss_common.c
         external/hss_compute.c
         external/hss_derive.c
         external/hss_generate.c
         external/hss_keygen.c
         external/hss_param.c
         external/hss_reserve.c
         external/hss_sign.c
         external/hss_sign_inc.c
         external/hss_thread_single.c
         external/hss_verify.c
         external/hss_verify_inc.c
         external/hss_zeroize.c
         external/lm_common.c
         external/lm_ots_common.c
         external/lm_ots_sign.c
         external/lm_ots_verify.c
         external/lm_verify.c
         external/sha256.c)

add_library(lms OBJECT ${SRCS})
target_include_directories(lms PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_compile_definitions(lms PRIVATE OQS
                                          LMS_EXPORT=
                                          WITH_SHA256_N32_W1
                                          WITH_SHA256_N32_W2
                                          WITH_SHA256_N32_W4
                                          WITH_SHA256_N32_W8
                                          WITH_SHA256_N32_H5
                                          WITH_SHA256_N32_H10
                                          WITH_SHA256_N32_H15
                                          WITH_SHA256_N32_H20
                                          WITH_SHA256_N32_H25
                                          )

