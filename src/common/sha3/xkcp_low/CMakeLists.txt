# SPDX-License-Identifier: MIT

set(_XKCP_LOW_OBJS "")
set(_XKCP_LOW_INCLUDE_DIRS "")

if(NOT OQS_DIST_BUILD AND OQS_USE_AVX2_INSTRUCTIONS AND CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
  add_library(xkcp_low_keccakp_1600_avx2 OBJECT KeccakP-1600/avx2/KeccakP-1600-AVX2.s)
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      target_compile_options(xkcp_low_keccakp_1600_avx2 PRIVATE -Wa,-defsym,macOS=1)
  endif()
  target_include_directories(xkcp_low_keccakp_1600_avx2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/avx2)
  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600_avx2>)
  set(_XKCP_LOW_INCLUDE_DIRS ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/avx2)

  add_library(xkcp_low_keccakp_1600times4_avx2 OBJECT KeccakP-1600times4/avx2/KeccakP-1600-times4-SIMD256.c)
  target_compile_options(xkcp_low_keccakp_1600times4_avx2 PRIVATE -mavx2)
  target_include_directories(xkcp_low_keccakp_1600times4_avx2 PRIVATE ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/avx2)
  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_avx2>)
  set(_XKCP_LOW_INCLUDE_DIRS ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/avx2)
else()
  add_library(xkcp_low_keccakp_1600_plain64 OBJECT KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c)
  target_include_directories(xkcp_low_keccakp_1600_plain64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/plain-64bits)
  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600_plain64>)
  set(_XKCP_LOW_INCLUDE_DIRS ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/plain-64bits)

  add_library(xkcp_low_keccakp_1600times4_plain64 OBJECT KeccakP-1600times4/serial/KeccakP-1600-times4-on1.c)
  target_include_directories(xkcp_low_keccakp_1600times4_plain64 PRIVATE ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/serial)
  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_plain64>)
  set(_XKCP_LOW_INCLUDE_DIRS ${_XKCP_LOW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/serial)
endif()

set(XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} PARENT_SCOPE)
set(XKCP_LOW_INCLUDE_DIRS ${_XKCP_LOW_INCLUDE_DIRS} PARENT_SCOPE)
