# SPDX-License-Identifier: MIT

set(_XMSS_OBJS "")

set(SRCS sig_stfl_xmss_sha256.c
)

add_library(xmss OBJECT ${SRCS})
set(_XMSS_OBJS ${_XMSS_OBJS} $<TARGET_OBJECTS:xmss>)
set(XMSS_OBJS ${_XMSS_OBJS} PARENT_SCOPE)
