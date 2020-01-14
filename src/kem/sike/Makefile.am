AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemsike.la

libkemsike_la_SOURCES = kem_sike.c P434/P434.c P434/P434_compressed.c P503/P503.c P503/P503_compressed.c P610/P610.c P610/P610_compressed.c P751/P751.c P751/P751_compressed.c

SIKE_FLAGS = -D_AMD64_

libkemsike_la_CFLAGS = $(AM_CFLAGS) $(SIKE_FLAGS) -w

P434/P434.c: sike.c sidh.c
P434/P434_compressed.c: sike.c sidh.c
P503/P503.c: sike.c sidh.c
P503/P503_compressed.c: sike.c sidh.c
P610/P610.c: sike.c sidh.c
P610/P610_compressed.c: sike.c sidh.c
P751/P751.c: sike.c sidh.c
P751/P751_compressed.c: sike.c sidh.c
