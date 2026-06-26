# SPDX-License-Identifier: MIT

set(_XKCP_LOW_OBJS "")

# XKCP is imported upstream code; don't promote its warnings to errors
# (newer compilers may flag constructs we shouldn't be patching locally).
set(_XKCP_LOW_WARN_AS_ERROR OFF)

# Determine which of the implementations we're building
if(OQS_DIST_X86_64_BUILD AND OQS_ENABLE_SHA3_xkcp_low_avx2)
  set(BUILD_PLAIN64 ON)
  set(BUILD_AVX2 ON)
elseif(OQS_ENABLE_SHA3_xkcp_low_avx2)
  set(BUILD_AVX2 ON)
else()
  set(BUILD_PLAIN64 ON)
endif()

if(BUILD_PLAIN64)
  add_library(xkcp_low_keccakp_1600_plain64 OBJECT KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c)
  set_target_properties(xkcp_low_keccakp_1600_plain64 PROPERTIES COMPILE_WARNING_AS_ERROR ${_XKCP_LOW_WARN_AS_ERROR})

  add_library(xkcp_low_keccakp_1600times4_serial OBJECT KeccakP-1600times4/serial/KeccakP-1600-times4-on1.c)
  target_include_directories(xkcp_low_keccakp_1600times4_serial PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/plain-64bits)
  set_target_properties(xkcp_low_keccakp_1600times4_serial PROPERTIES COMPILE_WARNING_AS_ERROR ${_XKCP_LOW_WARN_AS_ERROR})

  if(OQS_DIST_X86_64_BUILD)
    target_compile_definitions(xkcp_low_keccakp_1600_plain64 PRIVATE ADD_SYMBOL_SUFFIX)
    target_compile_definitions(xkcp_low_keccakp_1600times4_serial PRIVATE ADD_SYMBOL_SUFFIX)
  endif()

  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600_plain64>
                                       $<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_serial>)
endif()

# We currently do not have a SHA3 AVX2 implementation that works on Windows
if(BUILD_AVX2 AND CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
  add_library(xkcp_low_keccakp_1600_avx2 OBJECT KeccakP-1600/avx2/KeccakP-1600-AVX2.S)
  set_target_properties(xkcp_low_keccakp_1600_avx2 PROPERTIES COMPILE_WARNING_AS_ERROR ${_XKCP_LOW_WARN_AS_ERROR})

  add_library(xkcp_low_keccakp_1600times4_avx2 OBJECT KeccakP-1600times4/avx2/KeccakP-1600-times4-SIMD256.c)
  target_include_directories(xkcp_low_keccakp_1600times4_avx2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/avx2)
  target_compile_options(xkcp_low_keccakp_1600times4_avx2 PRIVATE -mavx2)
  set_target_properties(xkcp_low_keccakp_1600times4_avx2 PROPERTIES COMPILE_WARNING_AS_ERROR ${_XKCP_LOW_WARN_AS_ERROR})

  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(xkcp_low_keccakp_1600_avx2 PRIVATE old_gas_syntax no_plt)
  endif()

  if(OQS_DIST_X86_64_BUILD)
    target_compile_definitions(xkcp_low_keccakp_1600_avx2 PRIVATE ADD_SYMBOL_SUFFIX)
    target_compile_definitions(xkcp_low_keccakp_1600times4_avx2 PRIVATE ADD_SYMBOL_SUFFIX)
  endif()

  set(_XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} $<TARGET_OBJECTS:xkcp_low_keccakp_1600_avx2>
                                       $<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_avx2>)
endif()

set(XKCP_LOW_OBJS ${_XKCP_LOW_OBJS} PARENT_SCOPE)
