# SPDX-License-Identifier: MIT

include(CMakeDependentOption)

# Switch off all algs except for those passed in the alglist
function(filter_algs alglist)
  # Set every OQS_ENABLE_* variable =OFF unless one of the following conditions holds:
  #  1. the switch for one of the requested minimal build algorithm's family, e.g OQS_ENABLE_KEM_KYBER
  #  2. the switch for one of the requested algorithms, e.g. OQS_ENABLE_KEM_kyber_768.
  #  3. the switch for platform-specific ("_aesni" or "_avx2") implementation of
  #     one of the requested algorithms, e.g. OQS_ENABLE_KEM_kyber_768_avx2.

  get_cmake_property(_vars VARIABLES)
  foreach (_var ${_vars})
      if(_var MATCHES "^OQS_ENABLE_..._" AND NOT _var MATCHES "_AVAILABLE$" AND ${_var})
          set(${_var} OFF PARENT_SCOPE)
          # Case 1, family name
	  foreach (_alg ${ARGV0})
             string(TOUPPER ${_alg} upalg)
	     if("OQS_ENABLE_${upalg}" MATCHES "^${_var}")
                 set(${_var} ON PARENT_SCOPE)
             endif()
          endforeach()
          # Case 2, exact match
	  foreach (_alg ${ARGV0})
	     if(${_var}X STREQUAL "OQS_ENABLE_${_alg}X")
                 set(${_var} ON PARENT_SCOPE)
             endif()
          endforeach()
          # Case 3, platform specific
          string(REPLACE "_aesni" "" _var_base ${_var})
          string(REPLACE "_avx2" "" _var_base ${_var_base})
          string(REPLACE "_avx" "" _var_base ${_var_base})
          string(REPLACE "_aarch64" "" _var_base ${_var_base})
	  foreach (_alg ${ARGV0})
            if(${_var}_AVAILABLE)
              if(${_var_base}X STREQUAL ${_alg}X)
                  set(${_var} ON PARENT_SCOPE)
              endif()
            endif()
          endforeach()
      endif()
  endforeach()
  message(STATUS "Algorithms filtered for ${ARGV0}")
endfunction()

if(DEFINED OQS_KEM_DEFAULT)
    message(WARNING "OQS_KEM_DEFAULT not longer supported")
endif()
if(DEFINED OQS_SIG_DEFAULT)
    message(WARNING "OQS_SIG_DEFAULT not longer supported")
endif()

# Only enable OpenSSL by default on not-Windows
if(NOT WIN32)
    option(OQS_USE_OPENSSL "Enable OpenSSL usage" ON)
else()
    option(OQS_USE_OPENSSL "Enable OpenSSL usage" OFF)
endif()

# Use OpenSSL's AES only if no AESNI and x86 dist build is used.
# Reason: AESNI implementation better fits our incremental API.
cmake_dependent_option(OQS_USE_AES_OPENSSL "" ON "OQS_USE_OPENSSL; NOT OQS_DIST_X86_64_BUILD; NOT OQS_USE_AES_INSTRUCTIONS" OFF)
cmake_dependent_option(OQS_USE_SHA2_OPENSSL "" ON "OQS_USE_OPENSSL" OFF)
# Disable OpenSSL's SHA3 by default. The implementation is not complete
# enough to support our incremental API.
cmake_dependent_option(OQS_USE_SHA3_OPENSSL "" OFF "OQS_USE_OPENSSL" OFF)

# sanity check: Disable OpenSSL if not a single OpenSSL component define is on
cmake_dependent_option(OQS_USE_OPENSSL "" ON "OQS_USE_AES_OPENSSL OR OQS_USE_SHA2_OPENSSL OR OQS_USE_SHA3_OPENSSL" OFF)

option(OQS_DLOPEN_OPENSSL "Enable OpenSSL through dlopen" OFF)

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR OQS_USE_AVX2_INSTRUCTIONS)
    cmake_dependent_option(OQS_ENABLE_SHA3_xkcp_low_avx2 "" ON "NOT OQS_USE_SHA3_OPENSSL" OFF)
endif()
endif()

# BIKE is not supported on Windows, 32-bit ARM, X86, S390X (big endian) and PPC64 (big endian)
cmake_dependent_option(OQS_ENABLE_KEM_BIKE "Enable BIKE algorithm family" ON "NOT WIN32; NOT ARCH_ARM32v7; NOT ARCH_X86; NOT ARCH_S390X; NOT ARCH_PPC64" OFF)
# BIKE doesn't work on any 32-bit platform
if(CMAKE_SIZEOF_VOID_P MATCHES "4")
set(OQS_ENABLE_KEM_BIKE OFF)
endif()
cmake_dependent_option(OQS_ENABLE_KEM_bike_l1 "" ON "OQS_ENABLE_KEM_BIKE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_bike_l3 "" ON "OQS_ENABLE_KEM_BIKE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_bike_l5 "" ON "OQS_ENABLE_KEM_BIKE" OFF)

option(OQS_ENABLE_KEM_FRODOKEM "Enable Frodo algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_640_aes "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_640_shake "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_976_aes "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_976_shake "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_1344_aes "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_frodokem_1344_shake "" ON "OQS_ENABLE_KEM_FRODOKEM" OFF)

option(OQS_ENABLE_KEM_NTRUPRIME "Enable ntruprime algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_ntruprime_sntrup761 "" ON "OQS_ENABLE_KEM_NTRUPRIME" OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_ntruprime_sntrup761_avx2 "" ON "OQS_ENABLE_KEM_ntruprime_sntrup761" OFF)
endif()
endif()

##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ENABLE_BY_ALG_START
option(OQS_ENABLE_KEM_CLASSIC_MCELIECE "Enable classic_mceliece algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_348864 "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_348864f "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_460896 "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_460896f "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6688128 "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6688128f "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6960119 "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6960119f "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_8192128 "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_8192128f "" ON "OQS_ENABLE_KEM_CLASSIC_MCELIECE" OFF)

option(OQS_ENABLE_KEM_HQC "Enable hqc algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_hqc_128 "" ON "OQS_ENABLE_KEM_HQC" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_hqc_192 "" ON "OQS_ENABLE_KEM_HQC" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_hqc_256 "" ON "OQS_ENABLE_KEM_HQC" OFF)

option(OQS_ENABLE_KEM_KYBER "Enable kyber algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_kyber_512 "" ON "OQS_ENABLE_KEM_KYBER" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_kyber_768 "" ON "OQS_ENABLE_KEM_KYBER" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_kyber_1024 "" ON "OQS_ENABLE_KEM_KYBER" OFF)

option(OQS_ENABLE_KEM_ML_KEM "Enable ml_kem algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_512_ipd "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_512 "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_768_ipd "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_768 "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_1024_ipd "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)
cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_1024 "" ON "OQS_ENABLE_KEM_ML_KEM" OFF)

option(OQS_ENABLE_SIG_DILITHIUM "Enable dilithium algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_SIG_dilithium_2 "" ON "OQS_ENABLE_SIG_DILITHIUM" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_dilithium_3 "" ON "OQS_ENABLE_SIG_DILITHIUM" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_dilithium_5 "" ON "OQS_ENABLE_SIG_DILITHIUM" OFF)

option(OQS_ENABLE_SIG_ML_DSA "Enable ml_dsa algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_44_ipd "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_44 "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_65_ipd "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_65 "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_87_ipd "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_87 "" ON "OQS_ENABLE_SIG_ML_DSA" OFF)

option(OQS_ENABLE_SIG_FALCON "Enable falcon algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_SIG_falcon_512 "" ON "OQS_ENABLE_SIG_FALCON" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_falcon_1024 "" ON "OQS_ENABLE_SIG_FALCON" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_512 "" ON "OQS_ENABLE_SIG_FALCON" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_1024 "" ON "OQS_ENABLE_SIG_FALCON" OFF)

option(OQS_ENABLE_SIG_SPHINCS "Enable sphincs algorithm family" ON)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_128f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_128s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_192f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_192s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_256f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_256s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_128f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_128s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_192f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_192s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_256f_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_256s_simple "" ON "OQS_ENABLE_SIG_SPHINCS" OFF)
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ENABLE_BY_ALG_END

if((OQS_MINIMAL_BUILD STREQUAL "ON"))
   message(FATAL_ERROR "OQS_MINIMAL_BUILD option ${OQS_MINIMAL_BUILD} no longer supported")
endif()

if(NOT DEFINED OQS_ALGS_ENABLED OR OQS_ALGS_ENABLED STREQUAL "")
	set(OQS_ALGS_ENABLED "All")
endif()

if(NOT ((OQS_MINIMAL_BUILD STREQUAL "") OR (OQS_MINIMAL_BUILD STREQUAL "OFF")))
	filter_algs("${OQS_MINIMAL_BUILD}")
elseif (${OQS_ALGS_ENABLED} STREQUAL "STD")
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_LIST_STANDARDIZED_ALGS_START
	filter_algs("KEM_ml_kem_512;KEM_ml_kem_768;KEM_ml_kem_1024;SIG_ml_dsa_44;SIG_ml_dsa_65;SIG_ml_dsa_87;SIG_falcon_512;SIG_falcon_1024;SIG_falcon_padded_512;SIG_falcon_padded_1024;SIG_sphincs_sha2_128f_simple;SIG_sphincs_sha2_128s_simple;SIG_sphincs_sha2_192f_simple;SIG_sphincs_sha2_192s_simple;SIG_sphincs_sha2_256f_simple;SIG_sphincs_sha2_256s_simple;SIG_sphincs_shake_128f_simple;SIG_sphincs_shake_128s_simple;SIG_sphincs_shake_192f_simple;SIG_sphincs_shake_192s_simple;SIG_sphincs_shake_256f_simple;SIG_sphincs_shake_256s_simple")
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_LIST_STANDARDIZED_ALGS_END
elseif(${OQS_ALGS_ENABLED} STREQUAL "NIST_R4")
	filter_algs("KEM_classic_mceliece_348864;KEM_classic_mceliece_348864f;KEM_classic_mceliece_460896;KEM_classic_mceliece_460896f;KEM_classic_mceliece_6688128;KEM_classic_mceliece_6688128f;KEM_classic_mceliece_6960119;KEM_classic_mceliece_6960119f;KEM_classic_mceliece_8192128;KEM_classic_mceliece_8192128f;KEM_hqc_128;KEM_hqc_192;KEM_hqc_256;KEM_bike_l1;KEM_bike_l3;KEM_bike_l5")
else()
	message(STATUS "Alg enablement unchanged")
endif()


##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ENABLE_BY_ALG_CONDITIONAL_START
if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_348864_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_348864" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS AND OQS_USE_BMI1_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_348864f_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_348864f" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_460896_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_460896" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS AND OQS_USE_BMI1_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_460896f_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_460896f" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6688128_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_6688128" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS AND OQS_USE_BMI1_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6688128f_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_6688128f" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6960119_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_6960119" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS AND OQS_USE_BMI1_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_6960119f_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_6960119f" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_8192128_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_8192128" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS AND OQS_USE_BMI1_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_classic_mceliece_8192128f_avx2 "" ON "OQS_ENABLE_KEM_classic_mceliece_8192128f" OFF)
endif()
endif()



if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_512_avx2 "" ON "OQS_ENABLE_KEM_kyber_512" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if((OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS)))
if(((CMAKE_C_COMPILER_ID STREQUAL "GNU") AND (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((NOT (CMAKE_C_COMPILER_ID STREQUAL "GNU")) AND (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_512_aarch64 "" ON "OQS_ENABLE_KEM_kyber_512" OFF)
else()
    message(WARNING "  ARM optimizations are not fully supported on this compiler version.")
endif()
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_768_avx2 "" ON "OQS_ENABLE_KEM_kyber_768" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if((OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS)))
if(((CMAKE_C_COMPILER_ID STREQUAL "GNU") AND (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((NOT (CMAKE_C_COMPILER_ID STREQUAL "GNU")) AND (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_768_aarch64 "" ON "OQS_ENABLE_KEM_kyber_768" OFF)
else()
    message(WARNING "  ARM optimizations are not fully supported on this compiler version.")
endif()
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_1024_avx2 "" ON "OQS_ENABLE_KEM_kyber_1024" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if((OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS)))
if(((CMAKE_C_COMPILER_ID STREQUAL "GNU") AND (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9.4.0")) OR ((NOT (CMAKE_C_COMPILER_ID STREQUAL "GNU")) AND (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))))
    cmake_dependent_option(OQS_ENABLE_KEM_kyber_1024_aarch64 "" ON "OQS_ENABLE_KEM_kyber_1024" OFF)
else()
    message(WARNING "  ARM optimizations are not fully supported on this compiler version.")
endif()
endif()
endif()


if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_512_ipd_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_512_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_512_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_512" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_768_ipd_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_768_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_768_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_768" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_BMI2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_1024_ipd_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_1024_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_KEM_ml_kem_1024_avx2 "" ON "OQS_ENABLE_KEM_ml_kem_1024" OFF)
endif()
endif()


if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_2_avx2 "" ON "OQS_ENABLE_SIG_dilithium_2" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_2_aarch64 "" ON "OQS_ENABLE_SIG_dilithium_2" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_3_avx2 "" ON "OQS_ENABLE_SIG_dilithium_3" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_3_aarch64 "" ON "OQS_ENABLE_SIG_dilithium_3" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_5_avx2 "" ON "OQS_ENABLE_SIG_dilithium_5" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_dilithium_5_aarch64 "" ON "OQS_ENABLE_SIG_dilithium_5" OFF)
endif()
endif()


if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_44_ipd_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_44_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_44_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_44" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_65_ipd_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_65_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_65_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_65" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_POPCNT_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_87_ipd_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_87_ipd" OFF)
    cmake_dependent_option(OQS_ENABLE_SIG_ml_dsa_87_avx2 "" ON "OQS_ENABLE_SIG_ml_dsa_87" OFF)
endif()
endif()


if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_512_avx2 "" ON "OQS_ENABLE_SIG_falcon_512" OFF)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_512_aarch64 "" ON "OQS_ENABLE_SIG_falcon_512" OFF)
endif()
endif()

if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_1024_avx2 "" ON "OQS_ENABLE_SIG_falcon_1024" OFF)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_1024_aarch64 "" ON "OQS_ENABLE_SIG_falcon_1024" OFF)
endif()
endif()

if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_512_avx2 "" ON "OQS_ENABLE_SIG_falcon_padded_512" OFF)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_512_aarch64 "" ON "OQS_ENABLE_SIG_falcon_padded_512" OFF)
endif()
endif()

if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_1024_avx2 "" ON "OQS_ENABLE_SIG_falcon_padded_1024" OFF)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_ARM64_V8_BUILD OR (OQS_USE_ARM_NEON_INSTRUCTIONS AND OQS_USE_ARM_NEON_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_falcon_padded_1024_aarch64 "" ON "OQS_ENABLE_SIG_falcon_padded_1024" OFF)
endif()
endif()


if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_128f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_128f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_128s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_128s_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_192f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_192f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_192s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_192s_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_256f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_256f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_sha2_256s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_sha2_256s_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_128f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_128f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_128s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_128s_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_192f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_192f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_192s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_192s_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_256f_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_256f_simple" OFF)
endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
if(OQS_DIST_X86_64_BUILD OR (OQS_USE_AVX2_INSTRUCTIONS))
    cmake_dependent_option(OQS_ENABLE_SIG_sphincs_shake_256s_simple_avx2 "" ON "OQS_ENABLE_SIG_sphincs_shake_256s_simple" OFF)
endif()
endif()

##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ENABLE_BY_ALG_CONDITIONAL_END

# Set XKCP (Keccak) required for Sphincs AVX2 code even if OpenSSL3 SHA3 is used:
if (${OQS_ENABLE_SIG_SPHINCS} OR NOT ${OQS_USE_SHA3_OPENSSL})
    set(OQS_ENABLE_SHA3_xkcp_low ON)
else()
    set(OQS_ENABLE_SHA3_xkcp_low OFF)
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
    if(OQS_DIST_X86_64_BUILD OR OQS_USE_AVX2_INSTRUCTIONS)
        set(OQS_ENABLE_SHA3_xkcp_low_avx2 ON)
    else()
        set(OQS_ENABLE_SHA3_xkcp_low_avx2 OFF)
    endif()
endif()