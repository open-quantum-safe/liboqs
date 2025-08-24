# SPDX-License-Identifier: MIT

set(_HQC_OBJS "")

# -------------------------------
# Include directories
# -------------------------------
set(HQC_INCLUDE_DIRS
    oqs_hqc_adapter
    ref_hqc/src
    ref_hqc/src/common
    ref_hqc/src/common/hqc-1
    ref_hqc/src/common/hqc-3
    ref_hqc/src/common/hqc-5
    ref_hqc/src/ref
    ref_hqc/src/ref/hqc-1
    ref_hqc/src/ref/hqc-3
    ref_hqc/src/ref/hqc-5
    ref_hqc/lib/fips202 # FIPS202 headers
)

# -------------------------------
# FIPS202 library
# -------------------------------
add_library(fips202_obj OBJECT ref_hqc/lib/fips202/fips202.c)
target_include_directories(fips202_obj PRIVATE ref_hqc/lib/fips202)

# -------------------------------
# Common sources (exclude kem.c)
# -------------------------------
file(GLOB COMMON_SOURCES kem_hqc_128.c kem_hqc_192.c kem_hqc_256.c
     ref_hqc/src/common/*.c)
list(REMOVE_ITEM COMMON_SOURCES ref_hqc/src/common/kem.c)

# -------------------------------
# Common object library
# -------------------------------
add_library(common_obj OBJECT ${COMMON_SOURCES})
target_include_directories(common_obj PRIVATE ${HQC_INCLUDE_DIRS})
target_link_libraries(common_obj PRIVATE fips202_obj)

# -------------------------------
# Function to build kem.c per HQC level
# -------------------------------
function(add_kem_level level)
  add_library(kem_hqc_${level} OBJECT ref_hqc/src/common/kem.c)
  target_include_directories(
    kem_hqc_${level} PRIVATE ref_hqc/src/common/hqc-${level}
                             ${HQC_INCLUDE_DIRS})
  target_compile_definitions(kem_hqc_${level}
                             PRIVATE KEM_PREFIX=REF_HQC_${level})
  target_link_libraries(kem_hqc_${level} PRIVATE common_obj fips202_obj)
  target_compile_options(kem_hqc_${level} PRIVATE -O0)
endfunction()

# -------------------------------
# Ref object library
# -------------------------------
file(GLOB REF_SOURCES ref_hqc/src/ref/*.c)

add_library(ref_obj OBJECT ${REF_SOURCES})
target_include_directories(ref_obj PRIVATE ${HQC_INCLUDE_DIRS})
target_link_libraries(ref_obj PRIVATE common_obj fips202_obj)

list(APPEND _HQC_OBJS $<TARGET_OBJECTS:common_obj> $<TARGET_OBJECTS:ref_obj>
     $<TARGET_OBJECTS:fips202_obj>)

add_kem_level("1")
list(APPEND _HQC_OBJS $<TARGET_OBJECTS:kem_hqc_1>)

add_kem_level("3")
list(APPEND _HQC_OBJS $<TARGET_OBJECTS:kem_hqc_3>)

add_kem_level("5")
list(APPEND _HQC_OBJS $<TARGET_OBJECTS:kem_hqc_5>)

set(HQC_OBJS
    ${_HQC_OBJS}
    PARENT_SCOPE)
