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
         external/picnic2_impl.c
         external/picnic2_simulate.c
         external/picnic2_simulate_mul.c
         external/picnic2_tree.c
         external/picnic2_types.c
         external/sha3/KeccakHash.c
         external/sha3/KeccakSpongeWidth1600.c
         external/sha3/KeccakHashtimes4.c
         external/sha3/KeccakSpongeWidth1600times4.c
         external/sha3/opt64/KeccakP-1600-opt64.c
         external/sha3/opt64/KeccakP-1600-times4-on1.c)

add_library(picnic OBJECT ${SRCS})
target_include_directories(picnic PRIVATE external
                                          external/sha3
                                          external/sha3/opt64)
target_compile_definitions(picnic PRIVATE PICNIC_STATIC
                                          OPTIMIZED_LINEAR_LAYER_EVALUATION
                                          REDUCED_ROUND_KEY_COMPUTATION
                                          WITH_LOWMC_128_128_20
                                          WITH_LOWMC_192_192_30
                                          WITH_LOWMC_256_256_38
                                          WITH_OPT
                                          WITH_POPCNT
                                          HAVE_POSIX_MEMALIGN)

# TODO: We can't have one without the other
if(USE_AVX2_INSTRUCTIONS AND USE_SSE2_INSTRUCTIONS)
    target_compile_definitions(picnic PRIVATE WITH_AVX2)
    target_compile_definitions(picnic PRIVATE WITH_SSE2)
endif()
