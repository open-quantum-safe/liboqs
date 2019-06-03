AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemkyber.la
noinst_LTLIBRARIES += libkemkyber512.la libkemkyber768.la libkemkyber1024.la 

libkemkyber_la_LIBADD  = libkemkyber512.la libkemkyber768.la libkemkyber1024.la 
libkemkyber_la_SOURCES = 

libkemkyber512_la_SOURCES = kem_kyber512.c pqclean_kyber512_clean/cbd.c pqclean_kyber512_clean/indcpa.c pqclean_kyber512_clean/kem.c pqclean_kyber512_clean/ntt.c pqclean_kyber512_clean/poly.c pqclean_kyber512_clean/polyvec.c pqclean_kyber512_clean/reduce.c pqclean_kyber512_clean/verify.c pqclean_kyber512_clean/symmetric-fips202.c 
libkemkyber512_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemkyber768_la_SOURCES = kem_kyber768.c pqclean_kyber768_clean/cbd.c pqclean_kyber768_clean/indcpa.c pqclean_kyber768_clean/kem.c pqclean_kyber768_clean/ntt.c pqclean_kyber768_clean/poly.c pqclean_kyber768_clean/polyvec.c pqclean_kyber768_clean/reduce.c pqclean_kyber768_clean/verify.c pqclean_kyber768_clean/symmetric-fips202.c 
libkemkyber768_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemkyber1024_la_SOURCES = kem_kyber1024.c pqclean_kyber1024_clean/cbd.c pqclean_kyber1024_clean/indcpa.c pqclean_kyber1024_clean/kem.c pqclean_kyber1024_clean/ntt.c pqclean_kyber1024_clean/poly.c pqclean_kyber1024_clean/polyvec.c pqclean_kyber1024_clean/reduce.c pqclean_kyber1024_clean/verify.c pqclean_kyber1024_clean/symmetric-fips202.c 
libkemkyber1024_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

