AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemfrodokem.la
noinst_LTLIBRARIES += libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la

libkemfrodokem_la_LIBADD  = libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la
libkemfrodokem_la_SOURCES = 

libkemfrodokem640aes_la_SOURCES = kem_frodokem640aes.c pqclean_frodokem640aes_clean/kem.c pqclean_frodokem640aes_clean/matrix_aes.c pqclean_frodokem640aes_clean/noise.c pqclean_frodokem640aes_clean/util.c
libkemfrodokem640aes_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem640shake_la_SOURCES = kem_frodokem640shake.c pqclean_frodokem640shake_clean/kem.c pqclean_frodokem640shake_clean/matrix_shake.c pqclean_frodokem640shake_clean/noise.c pqclean_frodokem640shake_clean/util.c
libkemfrodokem640shake_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem976aes_la_SOURCES = kem_frodokem976aes.c pqclean_frodokem976aes_clean/kem.c pqclean_frodokem976aes_clean/matrix_aes.c pqclean_frodokem976aes_clean/noise.c pqclean_frodokem976aes_clean/util.c
libkemfrodokem976aes_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem976shake_la_SOURCES = kem_frodokem976shake.c pqclean_frodokem976shake_clean/kem.c pqclean_frodokem976shake_clean/matrix_shake.c pqclean_frodokem976shake_clean/noise.c pqclean_frodokem976shake_clean/util.c
libkemfrodokem976shake_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem1344aes_la_SOURCES = kem_frodokem1344aes.c pqclean_frodokem1344aes_clean/kem.c pqclean_frodokem1344aes_clean/matrix_aes.c pqclean_frodokem1344aes_clean/noise.c pqclean_frodokem1344aes_clean/util.c
libkemfrodokem1344aes_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemfrodokem1344shake_la_SOURCES = kem_frodokem1344shake.c pqclean_frodokem1344shake_clean/kem.c pqclean_frodokem1344shake_clean/matrix_shake.c pqclean_frodokem1344shake_clean/noise.c pqclean_frodokem1344shake_clean/util.c
libkemfrodokem1344shake_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims
