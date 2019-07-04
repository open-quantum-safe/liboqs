AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemsike.la

libkemsike_la_SOURCES = kem_sike.c P434/P434.c P503/P503.c P610/P610.c P751/P751.c

libkemsike_la_CFLAGS = $(AM_CFLAGS) -w

P434/P434.c: sike.c sidh.c
P503/P503.c: sike.c sidh.c
P610/P610.c: sike.c sidh.c
P751/P751.c: sike.c sidh.c
