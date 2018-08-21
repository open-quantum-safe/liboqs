AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemfrodokem.la

libkemfrodokem_la_SOURCES = kem_frodokem.c frodo640_aes.c frodo640_cshake.c frodo976_aes.c frodo976_cshake.c util.c

libkemfrodokem_la_CPPFLAGS = -I../../../include -I. -fPIC -w -D_FAST_GENERIC_ -D_AMD64_ -DNO_OPENSSL
libkemfrodokem_la_CPPFLAGS += $(AM_CPPFLAGS)

frodo640_aes.c: frodo_macrify.h frodo_macrify.c frodo640_params.h  kem.c
frodo640_cshake.c: frodo_macrify.h frodo_macrify.c frodo640_params.h  kem.c
frodo976_aes.c: frodo_macrify.h frodo_macrify.c frodo976_params.h  kem.c
frodo976_cshake.c: frodo_macrify.h frodo_macrify.c frodo976_params.h  kem.c
