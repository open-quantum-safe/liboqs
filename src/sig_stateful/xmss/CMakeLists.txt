# SPDX-License-Identifier: MIT
set(SRCS sig_stfl_xmss_sha256.c
         sig_stfl_xmss_sha512.c
         sig_stfl_xmss_shake256.c
         sig_stfl_xmss_shake512.c
         sig_stfl_xmssmt_h20.c
         sig_stfl_xmssmt_h40.c
         sig_stfl_xmssmt_h60.c
         sig_stfl_xmss_xmssmt_functions.c
         external/hash_address.c
         external/hash.c
         external/params.c
         external/utils.c
         external/wots.c
         external/xmss_commons.c
         external/xmss_core_fast.c
         external/xmss.c)

add_library(xmss OBJECT ${SRCS})
target_include_directories(xmss PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_compile_definitions(xmss PRIVATE OQS
                                          XMSS_EXPORT=
                                          WITH_SHA256_H10
                                          WITH_SHA256_H16
                                          WITH_SHA256_H20
                                          WITH_SHA512_H10
                                          WITH_SHA512_H16
                                          WITH_SHA512_H20
                                          WITH_SHAKE256_H10
                                          WITH_SHAKE256_H16
                                          WITH_SHAKE256_H20
                                          WITH_SHAKE512_H10
                                          WITH_SHAKE512_H16
                                          WITH_SHAKE512_H20
                                          WITH_SHA256_2_H20
                                          WITH_SHA256_4_H20
                                          WITH_SHA256_2_H40
                                          WITH_SHA256_4_H40
                                          WITH_SHA256_8_H40
                                          WITH_SHA256_3_H60
                                          WITH_SHA256_6_H60
                                          WITH_SHA256_12_H60
                                          )