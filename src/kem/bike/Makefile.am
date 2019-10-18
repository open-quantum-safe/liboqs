AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkembike.la
noinst_LTLIBRARIES += libkembike1_l1_cpa.la libkembike1_l3_cpa.la
noinst_LTLIBRARIES += libkembike1_l1_fo.la libkembike1_l3_fo.la

libkembike_la_LIBADD = libkembike1_l1_cpa.la libkembike1_l3_cpa.la
libkembike_la_LIBADD += libkembike1_l1_fo.la libkembike1_l3_fo.la

libkembike_la_SOURCES = kem_bike.c

COMMON_FLAGS = $(AM_CFLAGS) -include functions_renaming.h

BIKE_DIR=additional

COMMON_CSRCS  = $(BIKE_DIR)/decode.c $(BIKE_DIR)/gf2x_portable.c $(BIKE_DIR)/sampling.c
COMMON_CSRCS += $(BIKE_DIR)/parallel_hash.c $(BIKE_DIR)/secure_decode_portable.c 
COMMON_CSRCS += $(BIKE_DIR)/utilities.c $(BIKE_DIR)/aes_ctr_prf.c $(BIKE_DIR)/error.c
COMMON_CSRCS += $(BIKE_DIR)/gf2x_mul.c 

if ON_DARWIN
COMMON_CSRCS += $(BIKE_DIR)/sampling_portable.c $(BIKE_DIR)/converts_portable.c
COMMON_FLAGS += -DPORTABLE
else
if USE_AVX512F_INSTRUCTIONS
COMMON_CSRCS += $(BIKE_DIR)/converts.S $(BIKE_DIR)/red.S $(BIKE_DIR)/secure_decode_avx512.S
COMMON_CSRCS += $(BIKE_DIR)/sampling_avx512.S $(BIKE_DIR)/gf_mul.S $(BIKE_DIR)/gf_add.S 
COMMON_FLAGS += -DAVX512
else
if USE_AVX2_INSTRUCTIONS
COMMON_CSRCS += $(BIKE_DIR)/converts.S $(BIKE_DIR)/red.S $(BIKE_DIR)/secure_decode_avx2.S
COMMON_CSRCS += $(BIKE_DIR)/sampling_avx2.S $(BIKE_DIR)/gf_mul.S $(BIKE_DIR)/gf_add.S 
COMMON_FLAGS += -DAVX2
else
COMMON_CSRCS += $(BIKE_DIR)/sampling_portable.c $(BIKE_DIR)/converts_portable.c
COMMON_FLAGS += -DPORTABLE
endif
endif
endif

if USE_OPENSSL
COMMON_CSRCS += $(BIKE_DIR)/openssl_utils.c
COMMON_FLAGS += -DUSE_OPENSSL
endif

libkembike_la_CFLAGS = $(COMMON_FLAGS)
libkembike_la_LIBTOOLFLAGS = $(AM_LIBTOOLFLAGS)

libkembike1_l1_cpa_la_SOURCES = $(COMMON_CSRCS) $(BIKE_DIR)/kem_cpa.c
libkembike1_l3_cpa_la_SOURCES = $(COMMON_CSRCS) $(BIKE_DIR)/kem_cpa.c
libkembike1_l1_fo_la_SOURCES = $(COMMON_CSRCS) $(BIKE_DIR)/kem.c
libkembike1_l3_fo_la_SOURCES = $(COMMON_CSRCS) $(BIKE_DIR)/kem.c

libkembike1_l1_cpa_la_CFLAGS = $(COMMON_FLAGS) -DINDCPA=1 -DLEVEL=1 -DFUNC_PREFIX=OQS_KEM_bike1_l1_cpa
libkembike1_l3_cpa_la_CFLAGS = $(COMMON_FLAGS) -DINDCPA=1 -DLEVEL=3 -DFUNC_PREFIX=OQS_KEM_bike1_l3_cpa
libkembike1_l1_fo_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=1 -DFUNC_PREFIX=OQS_KEM_bike1_l1_fo
libkembike1_l3_fo_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=3 -DFUNC_PREFIX=OQS_KEM_bike1_l3_fo

libkembike1_l1_cpa_la_CCASFLAGS = $(libkembike1_l1_cpa_la_CFLAGS)
libkembike1_l3_cpa_la_CCASFLAGS = $(libkembike1_l3_cpa_la_CFLAGS)
libkembike1_l1_fo_la_CCASFLAGS = $(libkembike1_l1_fo_la_CFLAGS)
libkembike1_l3_fo_la_CCASFLAGS = $(libkembike1_l3_fo_la_CFLAGS)
