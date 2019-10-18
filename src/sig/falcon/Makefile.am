AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsigfalcon.la
noinst_LTLIBRARIES += libsigfalcon_512.la libsigfalcon_1024.la 

libsigfalcon_la_LIBADD  = libsigfalcon_512.la libsigfalcon_1024.la 
libsigfalcon_la_SOURCES = 

libsigfalcon_512_la_SOURCES = sig_falcon_512.c pqclean_falcon-512_clean/codec.c pqclean_falcon-512_clean/common.c pqclean_falcon-512_clean/fft.c pqclean_falcon-512_clean/fpr.c pqclean_falcon-512_clean/keygen.c pqclean_falcon-512_clean/pqclean.c pqclean_falcon-512_clean/rng.c pqclean_falcon-512_clean/sign.c pqclean_falcon-512_clean/vrfy.c 
libsigfalcon_512_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libsigfalcon_1024_la_SOURCES = sig_falcon_1024.c pqclean_falcon-1024_clean/codec.c pqclean_falcon-1024_clean/common.c pqclean_falcon-1024_clean/fft.c pqclean_falcon-1024_clean/fpr.c pqclean_falcon-1024_clean/keygen.c pqclean_falcon-1024_clean/pqclean.c pqclean_falcon-1024_clean/rng.c pqclean_falcon-1024_clean/sign.c pqclean_falcon-1024_clean/vrfy.c 
libsigfalcon_1024_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

