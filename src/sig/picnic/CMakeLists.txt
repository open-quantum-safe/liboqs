# SPDX-License-Identifier: MIT

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-missing-braces)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    if(CMAKE_C_COMPILER_VERSION VERSION_GREATER "3.9")
        add_compile_options(-Wno-unguarded-availability-new)
    else() #Wno-unguarded-availability-new won't be recognized
        add_compile_options(-Wno-error)
    endif()
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
    # ignore some warnings. picnic team confirmed these are safe to ignore; will be fixed eventually
    add_compile_options(/wd4244 /wd4267 /wd4334)
endif()

set(SRCS sig_picnic.c
         external/bitstream.c
         external/io.c
         external/lowmc.c
         external/lowmc_128_128_20.c
         external/lowmc_192_192_30.c
         external/lowmc_256_256_38.c
         external/lowmc_129_129_4.c
         external/lowmc_192_192_4.c
         external/lowmc_255_255_4.c
         external/mpc_lowmc.c
         external/mzd_additional.c
         external/picnic.c
         external/picnic_impl.c
         external/picnic_instances.c
         external/picnic3_impl.c
         external/picnic3_simulate.c
         external/picnic3_tree.c
         external/picnic3_types.c
         external/randomness.c)

# Avoid generating no symbols:
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   set(SRCS ${SRCS} external/cpu.c)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" AND
   OQS_USE_AVX2_INSTRUCTIONS AND
   OQS_USE_BMI2_INSTRUCTIONS)
    set(USE_AVX2 ON)
endif()

add_library(picnic OBJECT ${SRCS})
target_include_directories(picnic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_compile_definitions(picnic PRIVATE OQS
                                          PICNIC_STATIC
                                          WITH_ZKBPP
                                          WITH_UNRUH
                                          WITH_KKW
                                          WITH_LOWMC_128_128_20
                                          WITH_LOWMC_192_192_30
                                          WITH_LOWMC_256_256_38
                                          WITH_LOWMC_129_129_4
                                          WITH_LOWMC_192_192_4
                                          WITH_LOWMC_255_255_4
                                          WITH_OPT)

if(OQS_USE_SSE2_INSTRUCTIONS)
    target_compile_definitions(picnic PRIVATE WITH_SSE2)
    add_compile_options(-msse2)
endif()
if(USE_AVX2)
    target_compile_definitions(picnic PRIVATE WITH_AVX2)
    add_compile_options(picnic PRIVATE -mavx2)
endif()

