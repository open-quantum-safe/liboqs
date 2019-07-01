AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsigdilithium.la
noinst_LTLIBRARIES += libsigdilithium2.la libsigdilithium3.la libsigdilithium4.la 

libsigdilithium_la_LIBADD  = libsigdilithium2.la libsigdilithium3.la libsigdilithium4.la 
libsigdilithium_la_SOURCES = 

libsigdilithium2_la_SOURCES = sig_dilithium2.c pqclean_dilithium2_clean/sign.c pqclean_dilithium2_clean/polyvec.c pqclean_dilithium2_clean/poly.c pqclean_dilithium2_clean/packing.c pqclean_dilithium2_clean/ntt.c pqclean_dilithium2_clean/reduce.c pqclean_dilithium2_clean/rounding.c pqclean_dilithium2_clean/symmetric.c 
libsigdilithium2_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libsigdilithium3_la_SOURCES = sig_dilithium3.c pqclean_dilithium3_clean/sign.c pqclean_dilithium3_clean/polyvec.c pqclean_dilithium3_clean/poly.c pqclean_dilithium3_clean/packing.c pqclean_dilithium3_clean/ntt.c pqclean_dilithium3_clean/reduce.c pqclean_dilithium3_clean/rounding.c pqclean_dilithium3_clean/symmetric.c 
libsigdilithium3_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libsigdilithium4_la_SOURCES = sig_dilithium4.c pqclean_dilithium4_clean/sign.c pqclean_dilithium4_clean/polyvec.c pqclean_dilithium4_clean/poly.c pqclean_dilithium4_clean/packing.c pqclean_dilithium4_clean/ntt.c pqclean_dilithium4_clean/reduce.c pqclean_dilithium4_clean/rounding.c pqclean_dilithium4_clean/symmetric.c 
libsigdilithium4_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

