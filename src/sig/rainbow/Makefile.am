AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsigrainbow.la
noinst_LTLIBRARIES += libsigrainbow_Ia_classic.la 

libsigrainbow_la_LIBADD  = libsigrainbow_Ia_classic.la 
libsigrainbow_la_SOURCES = 

libsigrainbow_Ia_classic_la_SOURCES = sig_rainbow_Ia_classic.c pqclean_rainbowIa-classic_clean/blas_comm.c pqclean_rainbowIa-classic_clean/parallel_matrix_op.c pqclean_rainbowIa-classic_clean/rainbow.c pqclean_rainbowIa-classic_clean/rainbow_keypair.c pqclean_rainbowIa-classic_clean/rainbow_keypair_computation.c pqclean_rainbowIa-classic_clean/sign.c pqclean_rainbowIa-classic_clean/utils_hash.c pqclean_rainbowIa-classic_clean/utils_prng.c pqclean_rainbowIa-classic_clean/blas_u32.c pqclean_rainbowIa-classic_clean/gf.c 
libsigrainbow_Ia_classic_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

