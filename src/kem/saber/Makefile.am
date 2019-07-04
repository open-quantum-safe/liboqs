AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemsaber.la
noinst_LTLIBRARIES += libkemsaber_lightsaber.la libkemsaber_saber.la libkemsaber_firesaber.la 

libkemsaber_la_LIBADD  = libkemsaber_lightsaber.la libkemsaber_saber.la libkemsaber_firesaber.la 
libkemsaber_la_SOURCES = 

libkemsaber_lightsaber_la_SOURCES = kem_saber_lightsaber.c pqclean_lightsaber_clean/cbd.c pqclean_lightsaber_clean/kem.c pqclean_lightsaber_clean/pack_unpack.c pqclean_lightsaber_clean/poly.c pqclean_lightsaber_clean/poly_mul.c pqclean_lightsaber_clean/SABER_indcpa.c pqclean_lightsaber_clean/verify.c 
libkemsaber_lightsaber_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemsaber_saber_la_SOURCES = kem_saber_saber.c pqclean_saber_clean/cbd.c pqclean_saber_clean/kem.c pqclean_saber_clean/pack_unpack.c pqclean_saber_clean/poly.c pqclean_saber_clean/poly_mul.c pqclean_saber_clean/SABER_indcpa.c pqclean_saber_clean/verify.c 
libkemsaber_saber_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemsaber_firesaber_la_SOURCES = kem_saber_firesaber.c pqclean_firesaber_clean/cbd.c pqclean_firesaber_clean/kem.c pqclean_firesaber_clean/pack_unpack.c pqclean_firesaber_clean/poly.c pqclean_firesaber_clean/poly_mul.c pqclean_firesaber_clean/SABER_indcpa.c pqclean_firesaber_clean/verify.c 
libkemsaber_firesaber_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

