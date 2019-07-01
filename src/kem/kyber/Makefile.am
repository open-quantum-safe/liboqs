AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemkyber.la
noinst_LTLIBRARIES += libkemkyber_512.la libkemkyber_768.la libkemkyber_1024.la 

libkemkyber_la_LIBADD  = libkemkyber_512.la libkemkyber_768.la libkemkyber_1024.la 
libkemkyber_la_SOURCES = 

libkemkyber_512_la_SOURCES = kem_kyber_512.c pqclean_kyber_512_clean/cbd.c pqclean_kyber_512_clean/indcpa.c pqclean_kyber_512_clean/kem.c pqclean_kyber_512_clean/ntt.c pqclean_kyber_512_clean/poly.c pqclean_kyber_512_clean/polyvec.c pqclean_kyber_512_clean/reduce.c pqclean_kyber_512_clean/verify.c pqclean_kyber_512_clean/symmetric-fips202.c 
libkemkyber_512_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemkyber_768_la_SOURCES = kem_kyber_768.c pqclean_kyber_768_clean/cbd.c pqclean_kyber_768_clean/indcpa.c pqclean_kyber_768_clean/kem.c pqclean_kyber_768_clean/ntt.c pqclean_kyber_768_clean/poly.c pqclean_kyber_768_clean/polyvec.c pqclean_kyber_768_clean/reduce.c pqclean_kyber_768_clean/verify.c pqclean_kyber_768_clean/symmetric-fips202.c 
libkemkyber_768_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemkyber_1024_la_SOURCES = kem_kyber_1024.c pqclean_kyber_1024_clean/cbd.c pqclean_kyber_1024_clean/indcpa.c pqclean_kyber_1024_clean/kem.c pqclean_kyber_1024_clean/ntt.c pqclean_kyber_1024_clean/poly.c pqclean_kyber_1024_clean/polyvec.c pqclean_kyber_1024_clean/reduce.c pqclean_kyber_1024_clean/verify.c pqclean_kyber_1024_clean/symmetric-fips202.c 
libkemkyber_1024_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

