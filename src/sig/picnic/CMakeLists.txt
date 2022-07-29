# SPDX-License-Identifier: MIT

set(OBJS "")
list(APPEND SOURCES sig_picnic.c
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

add_library(picnic OBJECT ${SOURCES})
oqs_target_init(picnic)
oqs_add_c_flags(picnic REQUIRED PRIVATE ${OQS_C_FLAGS})
oqs_add_ld_flags(picnic PRIVATE ${OQS_LD_FLAGS})
target_include_directories(picnic PRIVATE ${OQS_INCLUDE_DIRS})

list(APPEND OBJS $<TARGET_OBJECTS:picnic>)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    oqs_add_c_flags(picnic REQUIRED PRIVATE "-Wno-missing-braces")
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    if(CMAKE_C_COMPILER_VERSION VERSION_GREATER "3.9")
        oqs_add_c_flags(picnic REQUIRED PRIVATE "-Wno-unguarded-availability-new")
    else() #Wno-unguarded-availability-new won't be recognized
        oqs_add_c_flags(picnic REQUIRED PRIVATE "-Wno-error")
    endif()
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
    # ignore some warnings. picnic team confirmed these are safe to ignore; will be fixed eventually
    add_compile_options(/wd4244 /wd4267 /wd4334)
    oqs_add_c_flags(picnic REQUIRED PRIVATE "/wd4244" "/wd4267" "/wd4334")
endif()
target_include_directories(picnic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_compile_definitions(picnic PRIVATE OQS
                                          PICNIC_EXPORT=
                                          WITH_ZKBPP
                                          WITH_UNRUH
                                          WITH_KKW
                                          WITH_LOWMC_128_128_20
                                          WITH_LOWMC_192_192_30
                                          WITH_LOWMC_256_256_38
                                          WITH_LOWMC_129_129_4
                                          WITH_LOWMC_192_192_4
                                          WITH_LOWMC_255_255_4)

if(OQS_ENABLE_SIG_picnic_avx2 OR OQS_ENABLE_SIG_picnic_sse2 OR OQS_ENABLE_SIG_picnic_neon)
    target_compile_definitions(picnic PRIVATE WITH_OPT)
endif()
if(OQS_ENABLE_SIG_picnic_sse2)
    target_compile_definitions(picnic PRIVATE WITH_SSE2)
endif()
if(OQS_ENABLE_SIG_picnic_avx2)
    target_compile_definitions(picnic PRIVATE WITH_AVX2)
endif()
if(OQS_ENABLE_SIG_picnic_neon)
  target_compile_definitions(picnic PRIVATE WITH_NEON)
endif()

set(OQS_PICNIC_OBJS "${OBJS}" PARENT_SCOPE)
