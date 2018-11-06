AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkembike.la
noinst_LTLIBRARIES += libkembike1_l1.la libkembike1_l3.la libkembike1_l5.la
noinst_LTLIBRARIES += libkembike2_l1.la libkembike2_l3.la libkembike2_l5.la
noinst_LTLIBRARIES += libkembike3_l1.la libkembike3_l3.la libkembike3_l5.la

libkembike_la_LIBADD = libkembike1_l1.la libkembike1_l3.la libkembike1_l5.la
libkembike_la_LIBADD += libkembike2_l1.la libkembike2_l3.la libkembike2_l5.la
libkembike_la_LIBADD += libkembike3_l1.la libkembike3_l3.la libkembike3_l5.la

libkembike_la_SOURCES = kem_bike.c

COMMON_FLAGS = $(AM_CFLAGS) -include functions_renaming.h
libkembike_la_CFLAGS = $(COMMON_FLAGS)
libkembike_la_LIBTOOLFLAGS = $(AM_LIBTOOLFLAGS)

#When AVX2 is supported also AVX512 is supported and we can use the additional implementation.
if BIKE_ADDITIONAL_IMPL
  BIKE_SRC_DIR=x86_64/
else
  BIKE_SRC_DIR=ref/
endif

COMMON_CSRCS = $(BIKE_SRC_DIR)/kem.c $(BIKE_SRC_DIR)/aes_ctr_prf.c $(BIKE_SRC_DIR)/parallel_hash.c
COMMON_CSRCS += $(BIKE_SRC_DIR)/sampling.c $(BIKE_SRC_DIR)/utilities.c $(BIKE_SRC_DIR)/decode.c $(BIKE_SRC_DIR)/openssl_utils.c


if BIKE_ADDITIONAL_IMPL
  COMMON_CSRCS += $(BIKE_SRC_DIR)/aes.c $(BIKE_SRC_DIR)/sha.c
  COMMON_CSRCS += $(BIKE_SRC_DIR)/sha-mb.c $(BIKE_SRC_DIR)/gf2x_mul.c $(BIKE_SRC_DIR)/vaes256_key_expansion.S
  COMMON_CSRCS += $(BIKE_SRC_DIR)/gf_add.S $(BIKE_SRC_DIR)/converts.S $(BIKE_SRC_DIR)/gf_mul.S
  COMMON_CSRCS += $(BIKE_SRC_DIR)/sha384_update_mb.S $(BIKE_SRC_DIR)/red.S
  COMMON_CSRCS += $(BIKE_SRC_DIR)/sampling_x86_64.S  $(BIKE_SRC_DIR)/decode_x86_64.S
else
  COMMON_CSRCS += $(BIKE_SRC_DIR)/conversions.c
endif

libkembike1_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l1_la_SOURCES = $(COMMON_CSRCS)
libkembike1_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l3_la_SOURCES = $(COMMON_CSRCS)
libkembike1_l5_la_SOURCES = $(COMMON_CSRCS)
libkembike2_l5_la_SOURCES = $(COMMON_CSRCS)
libkembike3_l5_la_SOURCES = $(COMMON_CSRCS)

libkembike1_l1_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=1 -DBIKE_VER=1 -DFUNC_PREFIX=OQS_KEM_bike1_l1
libkembike2_l1_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=1 -DBIKE_VER=2 -DFUNC_PREFIX=OQS_KEM_bike2_l1
libkembike3_l1_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=1 -DBIKE_VER=3 -DFUNC_PREFIX=OQS_KEM_bike3_l1
# Same flags for assembly files
libkembike1_l1_la_CCASFLAGS = $(libkembike1_l1_la_CFLAGS)
libkembike2_l1_la_CCASFLAGS = $(libkembike2_l1_la_CFLAGS)
libkembike3_l1_la_CCASFLAGS = $(libkembike3_l1_la_CFLAGS)

libkembike1_l3_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=3 -DBIKE_VER=1 -DFUNC_PREFIX=OQS_KEM_bike1_l3
libkembike2_l3_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=3 -DBIKE_VER=2 -DFUNC_PREFIX=OQS_KEM_bike2_l3
libkembike3_l3_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=3 -DBIKE_VER=3 -DFUNC_PREFIX=OQS_KEM_bike3_l3
libkembike1_l3_la_CCASFLAGS = $(libkembike1_l3_la_CFLAGS)
libkembike2_l3_la_CCASFLAGS = $(libkembike2_l3_la_CFLAGS)
libkembike3_l3_la_CCASFLAGS = $(libkembike3_l3_la_CFLAGS)

libkembike1_l5_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=5 -DBIKE_VER=1 -DFUNC_PREFIX=OQS_KEM_bike1_l5
libkembike2_l5_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=5 -DBIKE_VER=2 -DFUNC_PREFIX=OQS_KEM_bike2_l5
libkembike3_l5_la_CFLAGS = $(COMMON_FLAGS) -DLEVEL=5 -DBIKE_VER=3 -DFUNC_PREFIX=OQS_KEM_bike3_l5
libkembike1_l5_la_CCASFLAGS = $(libkembike1_l5_la_CFLAGS)
libkembike2_l5_la_CCASFLAGS = $(libkembike2_l5_la_CFLAGS)
libkembike3_l5_la_CCASFLAGS = $(libkembike3_l5_la_CFLAGS)
