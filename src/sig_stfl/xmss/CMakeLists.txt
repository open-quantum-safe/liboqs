# SPDX-License-Identifier: MIT

set(_XMSS_OBJS "")

set(SRCS external/hash.c
         external/hash_address.c
         external/params.c
         external/utils.c 
         external/wots.c 
         external/xmss.c
         external/xmss_commons.c
    )
    
if (OQS_ENABLE_SIG_STFL_xmss_sha256_h10)
    add_compile_definitions(OQS_ENABLE_SIG_STFL_xmss_sha256_h10)
    set (SRCS ${SRCS} sig_stfl_xmss_sha256_h10.c external/core_hash.c external/xmss_core.c)
endif()


add_library(xmss OBJECT ${SRCS})
set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss>)
set(XMSS_OBJS ${_XMSS_OBJS} PARENT_SCOPE)
