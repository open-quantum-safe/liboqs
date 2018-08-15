AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemsike.la

libkemsike_la_SOURCES = kem_sike.c P503/P503.c P751/P751.c

libkemsike_la_CFLAGS = $(AM_CFLAGS) -w

P503/P503.c: sike.c sidh.c
P751/P751.c: sike.c sidh.c
