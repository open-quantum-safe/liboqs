AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemkyber.la
noinst_LTLIBRARIES += libkemkyber_512_cca.la libkemkyber_768_cca.la libkemkyber_1024_cca.la

libkemkyber_la_LIBADD  = libkemkyber_512_cca.la libkemkyber_768_cca.la libkemkyber_1024_cca.la
libkemkyber_la_SOURCES = kem_kyber.c

COMMON_FLAGS = $(AM_CFLAGS) -include functions_renaming.h
libkemkyber_la_CFLAGS = $(COMMON_FLAGS)

KYBER_SRC_DIR = ref

KYBER_CSRCS  = $(KYBER_SRC_DIR)/cbd.c $(KYBER_SRC_DIR)/indcpa.c $(KYBER_SRC_DIR)/kem.c
KYBER_CSRCS += $(KYBER_SRC_DIR)/ntt.c $(KYBER_SRC_DIR)/poly.c $(KYBER_SRC_DIR)/polyvec.c
KYBER_CSRCS += $(KYBER_SRC_DIR)/precomp.c $(KYBER_SRC_DIR)/reduce.c $(KYBER_SRC_DIR)/verify.c

libkemkyber_512_cca_la_SOURCES  = $(KYBER_CSRCS)
libkemkyber_768_cca_la_SOURCES  = $(KYBER_CSRCS)
libkemkyber_1024_cca_la_SOURCES = $(KYBER_CSRCS)

libkemkyber_512_cca_la_CFLAGS  = $(COMMON_FLAGS) -DKYBER_K=2 -DFUNC_PREFIX=OQS_KEM_kyber_512_cca_kem
libkemkyber_768_cca_la_CFLAGS  = $(COMMON_FLAGS) -DKYBER_K=3 -DFUNC_PREFIX=OQS_KEM_kyber_768_cca_kem
libkemkyber_1024_cca_la_CFLAGS = $(COMMON_FLAGS) -DKYBER_K=4 -DFUNC_PREFIX=OQS_KEM_kyber_1024_cca_kem
