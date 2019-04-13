AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemfrodokem.la
noinst_LTLIBRARIES += libkemfrodokem640aes.la libkemfrodokem640shake.la

libkemfrodokem_la_LIBADD  = libkemfrodokem640aes.la libkemfrodokem640shake.la
libkemfrodokem_la_SOURCES = 

libkemfrodokem640aes_la_SOURCES = kem_frodokem640aes.c pqclean_frodokem640aes_clean/kem.c pqclean_frodokem640aes_clean/matrix_aes.c pqclean_frodokem640aes_clean/noise.c pqclean_frodokem640aes_clean/util.c
libkemfrodokem640aes_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem640shake_la_SOURCES = kem_frodokem640shake.c pqclean_frodokem640shake_clean/kem.c pqclean_frodokem640shake_clean/matrix_shake.c pqclean_frodokem640shake_clean/noise.c pqclean_frodokem640shake_clean/util.c
libkemfrodokem640shake_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims
