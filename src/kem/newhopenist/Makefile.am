AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemnewhopenist.la
noinst_LTLIBRARIES += libkemnewhopenist_512_cca.la libkemnewhopenist_1024_cca.la

libkemnewhopenist_la_LIBADD = libkemnewhopenist_512_cca.la libkemnewhopenist_1024_cca.la
libkemnewhopenist_la_SOURCES = kem_newhopenist.c

COMMON_FLAGS = $(AM_CFLAGS) -include functions_renaming.h
libkemnewhopenist_la_CFLAGS = $(COMMON_FLAGS)

NEWHOPE_SRC_DIR = optimized

NEWHOPE_CSRCS = $(NEWHOPE_SRC_DIR)/cpapke.c $(NEWHOPE_SRC_DIR)/kem.c $(NEWHOPE_SRC_DIR)/ntt.c
NEWHOPE_CSRCS += $(NEWHOPE_SRC_DIR)/poly.c $(NEWHOPE_SRC_DIR)/precomp.c $(NEWHOPE_SRC_DIR)/reduce.c
NEWHOPE_CSRCS += $(NEWHOPE_SRC_DIR)/verify.c

libkemnewhopenist_512_cca_la_SOURCES = $(NEWHOPE_CSRCS)
libkemnewhopenist_1024_cca_la_SOURCES = $(NEWHOPE_CSRCS)

libkemnewhopenist_512_cca_la_CFLAGS = $(COMMON_FLAGS) -DNEWHOPE_N=512 -DFUNC_PREFIX=OQS_KEM_newhope_512_cca_kem
libkemnewhopenist_1024_cca_la_CFLAGS = $(COMMON_FLAGS) -DNEWHOPE_N=1024 -DFUNC_PREFIX=OQS_KEM_newhope_1024_cca_kem
