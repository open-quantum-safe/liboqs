AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemledacrypt.la
noinst_LTLIBRARIES += libkemledakemlt12.la 

libkemledacrypt_la_LIBADD  = libkemledakemlt12.la 
libkemledacrypt_la_SOURCES = 

libkemledakemlt12_la_SOURCES = kem_ledakemlt12.c pqclean_ledakemlt12_leaktime/bf_decoding.c pqclean_ledakemlt12_leaktime/dfr_test.c pqclean_ledakemlt12_leaktime/gf2x_arith_mod_xPplusOne.c pqclean_ledakemlt12_leaktime/gf2x_arith.c pqclean_ledakemlt12_leaktime/H_Q_matrices_generation.c pqclean_ledakemlt12_leaktime/kem.c pqclean_ledakemlt12_leaktime/niederreiter.c pqclean_ledakemlt12_leaktime/rng.c 
libkemledakemlt12_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

