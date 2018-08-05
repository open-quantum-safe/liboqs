AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemfrodo.la

libkemfrodo_la_SOURCES = kem_frodo.c frodo640_aes.c frodo640_cshake.c frodo976_aes.c frodo976_cshake.c util.c

libkemfrodo_la_CFLAGS = $(AM_CFLAGS)

frodo640_aes.c: frodo_macrify.h frodo_macrify.c frodo640_params.h  kem.c
frodo640_cshake.c: frodo_macrify.h frodo_macrify.c frodo640_params.h  kem.c
frodo976_aes.c: frodo_macrify.h frodo_macrify.c frodo976_params.h  kem.c
frodo976_cshake.c: frodo_macrify.h frodo_macrify.c frodo976_params.h  kem.c
