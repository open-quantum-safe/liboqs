# SPDX-License-Identifier: MIT

set(_HQC_OBJS "")

# -------------------------------
# Include directories
# -------------------------------
set(HQC_INCLUDE_DIRS
    oqs_adapter
    hqc_ref/src
    hqc_ref/src/common
    hqc_ref/src/common/hqc-1
    hqc_ref/src/common/hqc-3
    hqc_ref/src/common/hqc-5
    hqc_ref/src/ref
    hqc_ref/src/ref/hqc-1
    hqc_ref/src/ref/hqc-3
    hqc_ref/src/ref/hqc-5
    hqc_ref/lib/fips202 # FIPS202 headers
)

# -------------------------------
# FIPS202 library
# -------------------------------
add_library(hqc_ref_fips202_obj OBJECT hqc_ref/lib/fips202/fips202.c)
target_include_directories(hqc_ref_fips202_obj PRIVATE hqc_ref/lib/fips202)

# -------------------------------
# Common sources (exclude kem.c)
# -------------------------------
file(GLOB COMMON_SOURCES kem_hqc_128.c kem_hqc_192.c kem_hqc_256.c
     hqc_ref/src/common/*.c)
list(REMOVE_ITEM COMMON_SOURCES hqc_ref/src/common/kem.c)

# -------------------------------
# Common object library
# -------------------------------
add_library(hqc_ref_common_obj OBJECT ${COMMON_SOURCES})
target_include_directories(hqc_ref_common_obj PRIVATE ${HQC_INCLUDE_DIRS})
target_link_libraries(hqc_ref_common_obj PRIVATE hqc_ref_fips202_obj)

# -------------------------------
# Function to build kem.c per HQC level
# -------------------------------
function(add_kem_level level)
  add_library(hqc_ref_kem_${level} OBJECT hqc_ref/src/common/kem.c)
  target_include_directories(
    hqc_ref_kem_${level} PRIVATE hqc_ref/src/common/hqc-${level}
                                 ${HQC_INCLUDE_DIRS})
  target_compile_definitions(hqc_ref_kem_${level}
                             PRIVATE KEM_PREFIX=REF_HQC_${level})
  target_link_libraries(hqc_ref_kem_${level} PRIVATE hqc_ref_common_obj
                                                     hqc_ref_fips202_obj)
  target_compile_options(hqc_ref_kem_${level} PRIVATE -O0)
endfunction()

# -------------------------------
# Ref object library
# -------------------------------
file(GLOB REF_SOURCES hqc_ref/src/ref/*.c)

add_library(hqc_ref_ref_obj OBJECT ${REF_SOURCES})
target_include_directories(hqc_ref_ref_obj PRIVATE ${HQC_INCLUDE_DIRS})
target_link_libraries(hqc_ref_ref_obj PRIVATE hqc_ref_common_obj
                                              hqc_ref_fips202_obj)

list(APPEND _HQC_OBJS $<TARGET_OBJECTS:hqc_ref_common_obj>
     $<TARGET_OBJECTS:hqc_ref_ref_obj> $<TARGET_OBJECTS:hqc_ref_fips202_obj>)

if(OQS_ENABLE_KEM_hqc_128)
  add_kem_level("1")
  list(APPEND _HQC_OBJS $<TARGET_OBJECTS:hqc_ref_kem_1>)
endif()

if(OQS_ENABLE_KEM_hqc_192)
  add_kem_level("3")
  list(APPEND _HQC_OBJS $<TARGET_OBJECTS:hqc_ref_kem_3>)
endif()

if(OQS_ENABLE_KEM_hqc_256)
  add_kem_level("5")
  list(APPEND _HQC_OBJS $<TARGET_OBJECTS:hqc_ref_kem_5>)
endif()

set(HQC_OBJS
    ${_HQC_OBJS}
    PARENT_SCOPE)
