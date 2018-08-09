AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkembike.la 
noinst_LTLIBRARIES += libkembike1_l1.la libkembike1_l3.la libkembike1_l5.la
noinst_LTLIBRARIES += libkembike2_l1.la libkembike2_l3.la libkembike2_l5.la
noinst_LTLIBRARIES += libkembike3_l1.la libkembike3_l3.la libkembike3_l5.la

libkembike_la_LIBADD = libkembike1_l1.la libkembike1_l3.la libkembike1_l5.la
libkembike_la_LIBADD += libkembike2_l1.la libkembike2_l3.la libkembike2_l5.la
libkembike_la_LIBADD += libkembike3_l1.la libkembike3_l3.la libkembike3_l5.la

libkembike_la_SOURCES = kem_bike.c 

COMMON_FLAGS = $(AM_CFLAGS) -include ref/functions_renaming.h
libkembike_la_CFLAGS = $(COMMON_FLAGS)

COMMON_CSRCS = ref/kem.c ref/aes_ctr_prf.c ref/parallel_hash.c ref/conversions.c 
COMMON_CSRCS += ref/sampling.c ref/utilities.c ref/decode.c ref/kem.c

libkembike1_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike1_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike1_l5_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l5_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l5_la_SOURCES = $(COMMON_CSRCS)

libkembike1_l1_la_CFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE1 -DFUNC_PREFIX=OQS_KEM_bike1_l1
libkembike2_l1_la_CFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE2 -DFUNC_PREFIX=OQS_KEM_bike2_l1
libkembike3_l1_la_CFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE3 -DFUNC_PREFIX=OQS_KEM_bike3_l1

libkembike1_l3_la_CFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE1 -DFUNC_PREFIX=OQS_KEM_bike1_l3
libkembike2_l3_la_CFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE2 -DFUNC_PREFIX=OQS_KEM_bike2_l3
libkembike3_l3_la_CFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE3 -DFUNC_PREFIX=OQS_KEM_bike3_l3

libkembike1_l5_la_CFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE1 -DFUNC_PREFIX=OQS_KEM_bike1_l5
libkembike2_l5_la_CFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE2 -DFUNC_PREFIX=OQS_KEM_bike2_l5
libkembike3_l5_la_CFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE3 -DFUNC_PREFIX=OQS_KEM_bike3_l5
