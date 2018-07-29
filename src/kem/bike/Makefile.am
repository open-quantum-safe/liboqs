AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkembike.la 
noinst_LTLIBRARIES += libcodebike1_l1.la libcodebike1_l3.la libcodebike1_l5.la
noinst_LTLIBRARIES += libcodebike2_l1.la libcodebike2_l3.la libcodebike2_l5.la
noinst_LTLIBRARIES += libcodebike3_l1.la libcodebike3_l3.la libcodebike3_l5.la

libkembike_la_LIBADD = libcodebike1_l1.la libcodebike1_l3.la libcodebike1_l5.la
libkembike_la_LIBADD += libcodebike2_l1.la libcodebike2_l3.la libcodebike2_l5.la
libkembike_la_LIBADD += libcodebike3_l1.la libcodebike3_l3.la libcodebike3_l5.la
libkembike_la_SOURCES = kem_bike.c 

CPPFLAGS = -I../../../include -I. -fPIC -w $(AM_CPPFLAGS)
libkembike_la_CPPFLAGS = $(CPPFLAGS)
libkembike1_l1_la_CPPFLAGS = $(CPPFLAGS)
libkembike1_l3_la_CPPFLAGS = $(CPPFLAGS)
libkembike1_l5_la_CPPFLAGS = $(CPPFLAGS)
libkembike2_l1_la_CPPFLAGS = $(CPPFLAGS)
libkembike2_l3_la_CPPFLAGS = $(CPPFLAGS)
libkembike2_l5_la_CPPFLAGS = $(CPPFLAGS)
libkembike3_l1_la_CPPFLAGS = $(CPPFLAGS)
libkembike3_l3_la_CPPFLAGS = $(CPPFLAGS)
libkembike3_l5_la_CPPFLAGS = $(CPPFLAGS)

COMMON_CSRCS = ref/kem.c ref/aes_ctr_prf.c ref/parallel_hash.c ref/conversions.c 
COMMON_CSRCS += ref/sampling.c ref/utilities.c ref/decode.c ref/kem.c

libcodebike1_l1_la_SOURCES = $(COMMON_CSRCS)
libcodebike2_l1_la_SOURCES = $(COMMON_CSRCS)
libcodebike3_l1_la_SOURCES = $(COMMON_CSRCS)
libcodebike1_l3_la_SOURCES = $(COMMON_CSRCS)
libcodebike2_l3_la_SOURCES = $(COMMON_CSRCS)
libcodebike3_l3_la_SOURCES = $(COMMON_CSRCS)
libcodebike1_l5_la_SOURCES = $(COMMON_CSRCS)
libcodebike2_l5_la_SOURCES = $(COMMON_CSRCS)
libcodebike3_l5_la_SOURCES = $(COMMON_CSRCS)

libcodebike1_l1_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE1 -DBIKE_FUNC_PREFIX=OQS_KEM_bike1_l1
libcodebike2_l1_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE2 -DBIKE_FUNC_PREFIX=OQS_KEM_bike2_l1
libcodebike3_l1_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM64 -DBIKE3 -DBIKE_FUNC_PREFIX=OQS_KEM_bike3_l1

libcodebike1_l3_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE1 -DBIKE_FUNC_PREFIX=OQS_KEM_bike1_l3
libcodebike2_l3_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE2 -DBIKE_FUNC_PREFIX=OQS_KEM_bike2_l3
libcodebike3_l3_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM96 -DBIKE3 -DBIKE_FUNC_PREFIX=OQS_KEM_bike3_l3

libcodebike1_l5_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE1 -DBIKE_FUNC_PREFIX=OQS_KEM_bike1_l5
libcodebike2_l5_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE2 -DBIKE_FUNC_PREFIX=OQS_KEM_bike2_l5
libcodebike3_l5_la_CPPFLAGS = $(COMMON_FLAGS) -DPARAM128 -DBIKE3 -DBIKE_FUNC_PREFIX=OQS_KEM_bike3_l5
