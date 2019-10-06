AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libpicnic_i.la

libpicnic_i_la_SOURCES = sig_picnic.c external/aligned_alloc.c external/bitstream.c external/cpu.c external/io.c external/lowmc.c external/lowmc_128_128_20.c external/lowmc_128_128_182.c external/lowmc_192_192_284.c external/lowmc_192_192_30.c external/lowmc_256_256_38.c external/lowmc_256_256_363.c external/mpc_lowmc.c external/mzd_additional.c external/picnic.c external/picnic_impl.c external/picnic2_impl.c external/picnic2_simulate.c external/picnic2_simulate_mul.c external/picnic2_tree.c external/picnic2_types.c external/sha3/KeccakHash.c external/sha3/KeccakSpongeWidth1600.c external/sha3/KeccakHashtimes4.c external/sha3/KeccakSpongeWidth1600times4.c external/sha3/opt64/KeccakP-1600-opt64.c external/sha3/opt64/KeccakP-1600-times4-on1.c

libpicnic_i_la_CFLAGS = -Iexternal -Iexternal/sha3 -Iexternal/sha3/opt64 -DPICNIC_STATIC -DOPTIMIZED_LINEAR_LAYER_EVALUATION -DREDUCED_ROUND_KEY_COMPUTATION -DWITH_LOWMC_128_128_20 -DWITH_LOWMC_192_192_30 -DWITH_LOWMC_256_256_38 -DWITH_OPT -DWITH_POPCNT

if USE_AVX2_INSTRUCTIONS
libpicnic_i_la_CFLAGS += -DWITH_AVX2
endif

if USE_SSE2_INSTRUCTIONS
libpicnic_i_la_CFLAGS += -DWITH_SSE2
endif

#from config
libpicnic_i_la_CFLAGS += -DHAVE_POSIX_MEMALIGN

libpicnic_i_la_CFLAGS += $(AM_CFLAGS) 

