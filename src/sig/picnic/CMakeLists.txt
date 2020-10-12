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
         external/compat.c
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
         external/randomness.c
         external/sha3/KeccakHash.c
         external/sha3/KeccakSponge.c
	 external/sha3/KeccakSpongetimes4.c
	 external/sha3/KeccakHashtimes4.c)

# Avoid generating no symbols:
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   set(SRCS ${SRCS} external/cpu.c)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" AND
   OQS_USE_AVX2_INSTRUCTIONS AND
   OQS_USE_BMI2_INSTRUCTIONS)
    set(USE_AVX2 ON)
endif()

if(USE_AVX2)
  set(SRCS ${SRCS}
           external/sha3/avx2/KeccakP-1600-AVX2.s
           external/sha3/avx2/KeccakP-1600-times4-SIMD256.c)
else()
  set(SRCS ${SRCS}
           external/sha3/opt64/KeccakP-1600-opt64.c
           external/sha3/opt64/KeccakP-1600-times4-on1.c)
endif()
add_library(picnic OBJECT ${SRCS})
target_include_directories(picnic PRIVATE external
                                          external/sha3)
if(USE_AVX2)
    target_include_directories(picnic PRIVATE external/sha3/avx2)
else()
    target_include_directories(picnic PRIVATE external/sha3/opt64)
endif()
target_compile_definitions(picnic PRIVATE PICNIC_STATIC
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
    target_compile_definitions(picnic PRIVATE WITH_AVX2
                                              WITH_KECCAK_X4)
    add_compile_options(picnic PRIVATE -mavx2)
endif()

