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

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" AND
   OQS_USE_AVX2_INSTRUCTIONS AND
   OQS_USE_BMI2_INSTRUCTIONS)
     set(PICNIC_WITH_AVX2 ON)
endif()

set(SRCS sig_picnic.c
         external/aligned_alloc.c
         external/bitstream.c
         external/cpu.c
         external/io.c
         external/lowmc.c
         external/lowmc_128_128_20.c
         external/lowmc_128_128_182.c
         external/lowmc_192_192_284.c
         external/lowmc_192_192_30.c
         external/lowmc_256_256_38.c
         external/lowmc_256_256_363.c
         external/mpc_lowmc.c
         external/mzd_additional.c
         external/picnic.c
         external/picnic_impl.c
         external/picnic_instances.c
         external/picnic2_impl.c
         external/picnic2_simulate.c
         external/picnic2_simulate_mul.c
         external/picnic2_tree.c
         external/picnic2_types.c
         external/sha3/KeccakHash.c
         external/sha3/KeccakSpongeWidth1600.c)
if(PICNIC_WITH_AVX2)
  set(SRCS ${SRCS}
           external/sha3/avx2/KeccakP-1600-AVX2.s
           external/sha3/avx2/KeccakP-1600-times4-SIMD256.c
           external/sha3/KeccakSpongeWidth1600times4.c
           external/sha3/KeccakHashtimes4.c)
else()
  set(SRCS ${SRCS}
           external/sha3/opt64/KeccakP-1600-opt64.c)
endif()
add_library(picnic OBJECT ${SRCS})
target_include_directories(picnic PRIVATE external
                                          external/sha3)
if(PICNIC_WITH_AVX2)
    target_include_directories(picnic PRIVATE external/sha3/avx2)
else()
    target_include_directories(picnic PRIVATE external/sha3/opt64)
endif()
target_compile_definitions(picnic PRIVATE PICNIC_STATIC
                                          OPTIMIZED_LINEAR_LAYER_EVALUATION
                                          REDUCED_ROUND_KEY_COMPUTATION
                                          WITH_ZKBPP
                                          WITH_KKW
                                          WITH_LOWMC_128_128_20
                                          WITH_LOWMC_192_192_30
                                          WITH_LOWMC_256_256_38
                                          WITH_OPT)
if(NOT WIN32)
    target_compile_definitions(picnic PRIVATE HAVE_POSIX_MEMALIGN)
endif()

if(OQS_USE_SSE2_INSTRUCTIONS)
    target_compile_definitions(picnic PRIVATE WITH_SSE2)
    add_compile_options(-msse2)
endif()
if(PICNIC_WITH_AVX2)
    target_compile_definitions(picnic PRIVATE WITH_AVX2
                                              WITH_KECCAK_X4)
    add_compile_options(picnic PRIVATE -mavx2)
endif()
if(OQS_USE_NEON_INSTRUCTIONS)
    target_compile_definitions(picnic PRIVATE WITH_NEON)
endif()

