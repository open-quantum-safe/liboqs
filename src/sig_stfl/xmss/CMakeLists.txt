# SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0

set(_XMSS_OBJS "")

set(SRCS external/core_hash.c
         external/hash.c
         external/hash_address.c
         external/params.c
         external/utils.c
         external/wots.c
         external/xmss.c
         external/xmss_commons.c
         external/xmss_core_fast.c
)

add_library(sig_stfl_xmss_secret_key_functions OBJECT sig_stfl_xmss_secret_key_functions.c)
set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:sig_stfl_xmss_secret_key_functions>)

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h10)
    add_library(xmss_sha256_h10 OBJECT sig_stfl_xmss_sha256_h10.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h10 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h10 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h10>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h16)
    add_library(xmss_sha256_h16 OBJECT sig_stfl_xmss_sha256_h16.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h16 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h16 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h16>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h20)
    add_library(xmss_sha256_h20 OBJECT sig_stfl_xmss_sha256_h20.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h20 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h20 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h20>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake128_h10)
    add_library(xmss_shake128_h10 OBJECT sig_stfl_xmss_shake128_h10.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake128_h10 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake128_h10 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake128_h10>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake128_h16)
    add_library(xmss_shake128_h16 OBJECT sig_stfl_xmss_shake128_h16.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake128_h16 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake128_h16 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake128_h16>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake128_h20)
    add_library(xmss_shake128_h20 OBJECT sig_stfl_xmss_shake128_h20.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake128_h20 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake128_h20 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake128_h20>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha512_h10)
    add_library(xmss_sha512_h10 OBJECT sig_stfl_xmss_sha512_h10.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha512_h10 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha512_h10 -DHASH=6)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha512_h10>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha512_h16)
    add_library(xmss_sha512_h16 OBJECT sig_stfl_xmss_sha512_h16.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha512_h16 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha512_h16 -DHASH=6)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha512_h16>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha512_h20)
    add_library(xmss_sha512_h20 OBJECT sig_stfl_xmss_sha512_h20.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha512_h20 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha512_h20 -DHASH=6)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha512_h20>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h10)
    add_library(xmss_shake256_h10 OBJECT sig_stfl_xmss_shake256_h10.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h10 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h10 -DHASH=7)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h10>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h16)
    add_library(xmss_shake256_h16 OBJECT sig_stfl_xmss_shake256_h16.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h16 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h16 -DHASH=7)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h16>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h20)
    add_library(xmss_shake256_h20 OBJECT sig_stfl_xmss_shake256_h20.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h20 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h20 -DHASH=7)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h20>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h10_192)
    add_library(xmss_sha256_h10_192 OBJECT sig_stfl_xmss_sha256_h10_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h10_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h10_192 -DHASH=1)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h10_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192)
    add_library(xmss_sha256_h16_192 OBJECT sig_stfl_xmss_sha256_h16_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h16_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h16_192 -DHASH=1)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h16_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192)
    add_library(xmss_sha256_h20_192 OBJECT sig_stfl_xmss_sha256_h20_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_sha256_h20_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_sha256_h20_192 -DHASH=1)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_sha256_h20_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h10_192)
    add_library(xmss_shake256_h10_192 OBJECT sig_stfl_xmss_shake256_h10_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h10_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h10_192 -DHASH=2)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h10_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192)
    add_library(xmss_shake256_h16_192 OBJECT sig_stfl_xmss_shake256_h16_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h16_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h16_192 -DHASH=2)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h16_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192)
    add_library(xmss_shake256_h20_192 OBJECT sig_stfl_xmss_shake256_h20_192.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h20_192 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h20_192 -DHASH=2)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h20_192>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h10_256)
    add_library(xmss_shake256_h10_256 OBJECT sig_stfl_xmss_shake256_h10_256.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h10_256 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h10_256 -DHASH=5)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h10_256>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256)
    add_library(xmss_shake256_h16_256 OBJECT sig_stfl_xmss_shake256_h16_256.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h16_256 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h16_256 -DHASH=5)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h16_256>)
endif()

if (OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256)
    add_library(xmss_shake256_h20_256 OBJECT sig_stfl_xmss_shake256_h20_256.c sig_stfl_xmss_functions.c ${SRCS})
    target_compile_options(xmss_shake256_h20_256 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmss_shake256_h20_256 -DHASH=5)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss_shake256_h20_256>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2)
    add_library(xmssmt_sha256_h20_2 OBJECT sig_stfl_xmssmt_sha256_h20_2.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h20_2 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h20_2 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h20_2>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4)
    add_library(xmssmt_sha256_h20_4 OBJECT sig_stfl_xmssmt_sha256_h20_4.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h20_4 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h20_4 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h20_4>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2)
    add_library(xmssmt_sha256_h40_2 OBJECT sig_stfl_xmssmt_sha256_h40_2.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h40_2 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h40_2 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h40_2>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4)
    add_library(xmssmt_sha256_h40_4 OBJECT sig_stfl_xmssmt_sha256_h40_4.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h40_4 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h40_4 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h40_4>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8)
    add_library(xmssmt_sha256_h40_8 OBJECT sig_stfl_xmssmt_sha256_h40_8.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h40_8 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h40_8 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h40_8>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3)
    add_library(xmssmt_sha256_h60_3 OBJECT sig_stfl_xmssmt_sha256_h60_3.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h60_3 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h60_3 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h60_3>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6)
    add_library(xmssmt_sha256_h60_6 OBJECT sig_stfl_xmssmt_sha256_h60_6.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h60_6 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h60_6 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h60_6>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12)
    add_library(xmssmt_sha256_h60_12 OBJECT sig_stfl_xmssmt_sha256_h60_12.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_sha256_h60_12 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_sha256_h60_12 -DHASH=3)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_sha256_h60_12>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2)
    add_library(xmssmt_shake128_h20_2 OBJECT sig_stfl_xmssmt_shake128_h20_2.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h20_2 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h20_2 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h20_2>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4)
    add_library(xmssmt_shake128_h20_4 OBJECT sig_stfl_xmssmt_shake128_h20_4.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h20_4 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h20_4 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h20_4>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2)
    add_library(xmssmt_shake128_h40_2 OBJECT sig_stfl_xmssmt_shake128_h40_2.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h40_2 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h40_2 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h40_2>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4)
    add_library(xmssmt_shake128_h40_4 OBJECT sig_stfl_xmssmt_shake128_h40_4.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h40_4 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h40_4 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h40_4>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8)
    add_library(xmssmt_shake128_h40_8 OBJECT sig_stfl_xmssmt_shake128_h40_8.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h40_8 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h40_8 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h40_8>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3)
    add_library(xmssmt_shake128_h60_3 OBJECT sig_stfl_xmssmt_shake128_h60_3.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h60_3 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h60_3 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h60_3>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6)
    add_library(xmssmt_shake128_h60_6 OBJECT sig_stfl_xmssmt_shake128_h60_6.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h60_6 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h60_6 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h60_6>)
endif()

if (OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12)
    add_library(xmssmt_shake128_h60_12 OBJECT sig_stfl_xmssmt_shake128_h60_12.c sig_stfl_xmssmt_functions.c ${SRCS})
    target_compile_options(xmssmt_shake128_h60_12 PRIVATE -DXMSS_PARAMS_NAMESPACE=xmssmt_shake128_h60_12 -DHASH=4)
    set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmssmt_shake128_h60_12>)
endif()

set(XMSS_OBJS ${_XMSS_OBJS} PARENT_SCOPE)
