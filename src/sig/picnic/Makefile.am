AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libpicnic_i.la

libpicnic_i_la_SOURCES = sig_picnic.c external/aligned_alloc.c external/bitstream.c external/cpu.c external/io.c external/lowmc.c external/lowmc_pars.c external/lowmc_128_128_20.c external/lowmc_192_192_30.c external/lowmc_256_256_38.c external/mpc_lowmc.c external/mzd_additional.c external/picnic.c external/picnic_impl.c external/sha3/KeccakHash.c external/sha3/KeccakSpongeWidth1600.c external/sha3/opt64/KeccakP-1600-opt64.c

libpicnic_i_la_CFLAGS = -Iexternal -Iexternal/sha3 -Iexternal/sha3/opt64 -DOPTIMIZED_LINEAR_LAYER_EVALUATION -DREDUCED_ROUND_KEY_COMPUTATION -DWITH_LOWMC_128_128_20 -DWITH_LOWMC_192_192_30 -DWITH_LOWMC_256_256_38 -DWITH_OPT -DWITH_POPCNT

if USE_AVX2_INSTRUCTIONS
libpicnic_i_la_CFLAGS += -DWITH_AVX2
endif

if USE_SSE2_INSTRUCTIONS
libpicnic_i_la_CFLAGS += -DWITH_SSE2
endif

#from config
libpicnic_i_la_CFLAGS += -DHAVE_ALIGNED_ALLOC -DHAVE_POSIX_MEMALIGN -DHAVE_MEMALIGN

libpicnic_i_la_CFLAGS += $(AM_CFLAGS) 

