AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemnewhope.la 
noinst_LTLIBRARIES += libkemnewhope_512cca.la 

#libkemnewhope_1024cca.la

libkemnewhope_la_LIBADD = libkemnewhope_512cca.la
#libkemnewhope_1024cca.la
libkemnewhope_la_SOURCES = kem_newhope.c

COMMON_FLAGS = $(AM_CFLAGS) -include functions_renaming.h
libkemnewhope_la_CFLAGS = $(COMMON_FLAGS)

NEWHOPE_SRC_DIR = optimized

NEWHOPE_CSRCS = $(NEWHOPE_SRC_DIR)/cpapke.c $(NEWHOPE_SRC_DIR)/kem.c $(NEWHOPE_SRC_DIR)/ntt.c
NEWHOPE_CSRCS += $(NEWHOPE_SRC_DIR)/poly.c $(NEWHOPE_SRC_DIR)/precomp.c $(NEWHOPE_SRC_DIR)/reduce.c
NEWHOPE_CSRCS += $(NEWHOPE_SRC_DIR)/verify.c

libkemnewhope_512cca_la_SOURCES = $(NEWHOPE_CSRCS)
#libkemnewhope_1024cca_la_SOURCES = $(NEWHOPE_CSRCS)

libkemnewhope_512cca_la_CFLAGS = $(COMMON_FLAGS) -DNEWHOPE_N=512 -DFUNC_PREFIX=OQS_KEM_newhope_512cca
#libkemnewhope_1024cca_la_CFLAGS = $(COMMON_FLAGS) -DNEWHOPE_N=1024 -DFUNC_PREFIX=OQS_KEM_newhope_1024cca



