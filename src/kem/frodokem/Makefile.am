AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemfrodokem.la
noinst_LTLIBRARIES += libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la 

libkemfrodokem_la_LIBADD  = libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la 
libkemfrodokem_la_SOURCES = 

libkemfrodokem640aes_la_SOURCES = kem_frodokem640aes.c external/frodo640aes.c
libkemfrodokem640aes_la_CFLAGS  = $(AM_CFLAGS)

libkemfrodokem640shake_la_SOURCES = kem_frodokem640shake.c external/frodo640shake.c
libkemfrodokem640shake_la_CFLAGS  = $(AM_CFLAGS)
 
libkemfrodokem976aes_la_SOURCES = kem_frodokem976aes.c external/frodo976aes.c
libkemfrodokem976aes_la_CFLAGS  = $(AM_CFLAGS)
 
libkemfrodokem976shake_la_SOURCES = kem_frodokem976shake.c external/frodo976shake.c
libkemfrodokem976shake_la_CFLAGS  = $(AM_CFLAGS)

libkemfrodokem1344aes_la_SOURCES = kem_frodokem1344aes.c external/frodo1344aes.c
libkemfrodokem1344aes_la_CFLAGS  = $(AM_CFLAGS)

libkemfrodokem1344shake_la_SOURCES = kem_frodokem1344shake.c external/frodo1344shake.c
libkemfrodokem1344shake_la_CFLAGS  = $(AM_CFLAGS)
