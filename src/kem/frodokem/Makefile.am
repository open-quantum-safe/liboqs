AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemfrodokem.la
noinst_LTLIBRARIES += libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la 

libkemfrodokem_la_LIBADD  = libkemfrodokem640aes.la libkemfrodokem640shake.la libkemfrodokem976aes.la libkemfrodokem976shake.la libkemfrodokem1344aes.la libkemfrodokem1344shake.la 
libkemfrodokem_la_SOURCES = 

libkemfrodokem640aes_la_SOURCES = kem_frodokem640aes.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem640aes_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo640aes.h"

libkemfrodokem640shake_la_SOURCES = kem_frodokem640shake.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem640shake_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo640shake.h"
 
libkemfrodokem976aes_la_SOURCES = kem_frodokem976aes.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem976aes_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo976aes.h"
 
libkemfrodokem976shake_la_SOURCES = kem_frodokem976shake.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem976shake_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo976shake.h"

libkemfrodokem1344aes_la_SOURCES = kem_frodokem1344aes.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem1344aes_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo1344aes.h"

libkemfrodokem1344shake_la_SOURCES = kem_frodokem1344shake.c external/kem.c external/noise.c external/util.c external/frodo_macrify_reference.c
libkemfrodokem1344shake_la_CFLAGS  = $(AM_CFLAGS) -include "external/frodo1344shake.h"
