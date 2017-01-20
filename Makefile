ifndef CC
	CC=cc
endif

AR=ar rcs
CURL=curl
RANLIB=ranlib
LN=ln -s
ECHO=echo

CFLAGS= -O3 -std=gnu11 -Wpedantic -Wall -Wextra -DCONSTANT_TIME 
LDFLAGS= -lm 
INCLUDES= -Iinclude

UNAME_S := $(shell uname -s)

ifdef ARCH
	CFLAGS += $(ARCH)
else
	CFLAGS += -march=x86-64
	ifeq ($(UNAME_S),Linux)
		CFLAGS += -DSIDH_ASM
	endif
endif

ifdef AES_NI
	AES_NI_LOCAL=$(AES_NI)
else
	AES_NI_LOCAL=1
endif
ifeq ($(AES_NI_LOCAL),1)
CFLAGS += -maes -msse2
else
CFLAGS += -DAES_DISABLE_NI
endif

UNAME_S := $(shell uname -s)
ifdef USE_OPENSSL
	CFLAGS += -DUSE_OPENSSL
	ifeq ($(UNAME_S),Linux)
		OPENSSL_DIR=/usr
	endif
	ifeq ($(UNAME_S),Darwin)
		OPENSSL_DIR=/usr/local/opt/openssl
	endif
	INCLUDES += -I$(OPENSSL_DIR)/include
	LDFLAGS += -L$(OPENSSL_DIR)/lib -lcrypto
endif

ifeq ($(UNAME_S),Darwin)
	INCLUDES += -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include
	ifdef ENABLE_CODE_MCBITS
		INCLUDES += -I/usr/local/include
		LDFLAGS += -L/usr/local/lib
	endif
endif

ifdef ENABLE_CODE_MCBITS
	CFLAGS += -DENABLE_CODE_MCBITS
	LDFLAGS += -lsodium
endif

.PHONY: all check clean prettyprint

all: links lib tests

objs/%.o: src/%.c | links
	@mkdir -p $(@D)
	@$(CC) -c  $(CFLAGS) $(INCLUDES) $< -o $@
	@$(ECHO) "CC $<"

links:
	@$(RM) -r include/oqs
	@mkdir -p include/oqs
	@$(LN) ../../src/crypto/aes/aes.h include/oqs
	@$(LN) ../../src/crypto/sha3/sha3.h include/oqs
	@$(LN) ../../src/kex/kex.h include/oqs
	@$(LN) ../../src/kex_rlwe_bcns15/kex_rlwe_bcns15.h include/oqs
	@$(LN) ../../src/kex_rlwe_newhope/kex_rlwe_newhope.h include/oqs
	@$(LN) ../../src/kex_rlwe_msrln16/kex_rlwe_msrln16.h include/oqs
ifdef ENABLE_CODE_MCBITS
	@$(LN) ../../src/kex_code_mcbits/kex_code_mcbits.h include/oqs
endif
	@$(LN) ../../src/kex_lwe_frodo/kex_lwe_frodo.h include/oqs
	@$(LN) ../../src/kex_sidh_cln16/kex_sidh_cln16.h include/oqs
	@$(LN) ../../src/crypto/rand/rand.h include/oqs
	@$(LN) ../../src/crypto/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs
	@$(LN) ../../src/crypto/rand_urandom_aesctr/rand_urandom_aesctr.h include/oqs
	@$(LN) ../../src/common/common.h include/oqs
	@$(LN) ../../src/kex_sidh_iqc_ref/kex_sidh_iqc_ref.h include/oqs

# RAND_URANDOM_CHACHA
RAND_URANDOM_CHACHA_OBJS :=  $(addprefix objs/crypto/rand_urandom_chacha20/, rand_urandom_chacha20.o)
$(RAND_URANDOM_CHACHA_OBJS): src/crypto/rand_urandom_chacha20/rand_urandom_chacha20.h

# RAND_URANDOM_AESCTR
RAND_URANDOM_AESCTR_OBJS :=  $(addprefix objs/crypto/rand_urandom_aesctr/, rand_urandom_aesctr.o)
$(RAND_URANDOM_AESCTR_OBJS): src/crypto/rand_urandom_aesctr/rand_urandom_aesctr.h

# RAND
objs/crypto/rand/rand.o: src/crypto/rand/rand.h

# KEX_RLWE_BCNS15
KEX_RLWE_BCNS15_OBJS := $(addprefix objs/kex_rlwe_bcns15/, fft.o kex_rlwe_bcns15.o rlwe.o rlwe_kex.o)
KEX_RLWE_BCNS15_HEADERS := $(addprefix src/kex_rlwe_bcns15/, kex_rlwe_bcns15.h local.h rlwe_a.h rlwe_table.h)
$(KEX_RLWE_BCNS15_OBJS): $(KEX_RLWE_BCNS15_HEADERS)

# KEX_NEWHOPE
KEX_RLWE_NEWHOPE_OBJS := $(addprefix objs/kex_rlwe_newhope/, kex_rlwe_newhope.o)
KEX_RLWE_NEWHOPE_HEADERS := $(addprefix src/kex_rlwe_newhope/, kex_rlwe_newhope.h newhope.c params.h poly.c precomp.c)
$(KEX_RLWE_NEWHOPE_OBJS): $(KEX_RLWE_NEWHOPE_HEADERS)

# KEX_RLWE_MSRLN16
KEX_RLWE_MSRLN16_OBJS := $(addprefix objs/kex_rlwe_msrln16/, kex_rlwe_msrln16.o LatticeCrypto_kex.o ntt_constants.o)
KEX_RLWE_MSRLN16_HEADERS := $(addprefix src/kex_rlwe_msrln16/, LatticeCrypto.h LatticeCrypto_priv.h kex_rlwe_msrln16.h )
$(KEX_RLWE_MSRLN16_OBJS): $(KEX_RLWE_MSRLN16_HEADERS)

# KEX_LWE_FRODO
KEX_LWE_FRODO_OBJS := $(addprefix objs/kex_lwe_frodo/, lwe.o kex_lwe_frodo.o lwe_noise.o)
KEX_LWE_FRODO_HEADERS := $(addprefix src/kex_lwe_frodo/, kex_lwe_frodo.h local.h kex_lwe_frodo_macrify.c lwe_macrify.c)
$(KEX_LWE_FRODO_OBJS): $(KEX_LWE_FRODO_HEADERS)

# KEX_SIDH_CLN16
#ifneq (,$(findstring SIDH_ASM,$(CFLAGS)))
objs/kex_sidh_cln16/fp_x64_asm.o: src/kex_sidh_cln16/AMD64/fp_x64_asm.S
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ src/kex_sidh_cln16/AMD64/fp_x64_asm.S
	@$(ECHO) "CC $<"
KEX_SIDH_CLN16_ASM_OBJS = fp_x64_asm.o
#endif
KEX_SIDH_CLN16_OBJS := $(addprefix objs/kex_sidh_cln16/, ec_isogeny.o fpx.o kex_sidh_cln16.o SIDH.o sidh_kex.o SIDH_setup.o validate.o $(KEX_SIDH_CLN16_ASM_OBJS))
KEX_SIDH_CLN16_HEADERS := $(addprefix src/kex_sidh_cln16/, kex_sidh_cln16.h SIDH.h)
$(KEX_SIDH_CLN16_OBJS): $(KEX_SIDH_CLN16_HEADERS)

# KEX_CODE_MCBITS
KEX_CODE_MCBITS_SRC := src/kex_code_mcbits/external/operations.c
KEX_CODE_MCBITS_SRC += $(wildcard src/kex_code_mcbits/*.c)
KEX_CODE_MCBITS_OBJS := $(patsubst src/%.c, objs/%.o, $(KEX_CODE_MCBITS_SRC))
KEX_CODE_MCBITS_HEADERS := $(wildcard src/kex_code_mcbits/external/*.h)
KEX_CODE_MCBITS_HEADERS += $(wildcard src/kex_code_mcbits/*.h)
$(KEX_CODE_MCBITS_OBJS): $(KEX_CODE_MCBITS_HEADERS)

# KEX_SIDH_IQC_REF
KEX_SIDH_IQC_REF_OBJS := $(addprefix objs/kex_sidh_iqc_ref/, kex_sidh_iqc_ref.o sidh_elliptic_curve.o sidh_elliptic_curve_dlp.o sidh_isogeny.o sidh_private_key.o sidh_public_key.o sidh_public_key_encryption.o sidh_public_key_validation.o sidh_public_param.o sidh_quadratic_ext.o sidh_shared_key.o sidh_util.o)
KEX_SIDH_IQC_REF_HEADERS := $(addprefix src/kex_sidh_iqc_ref/, kex_sidh_iqc_ref.h sidh_elliptic_curve.h sidh_elliptic_curve_dlp.h sidh_isogeny.h sidh_private_key.h sidh_public_key.h sidh_public_key_encryption.h sidh_public_key_validation.h sidh_public_param.h sidh_quadratic_ext.h sidh_shared_key.h sidh_util.h)
$(KEX_SIDH_IQC_REF_OBJS): $(KEX_SIDH_IQC_REF_HEADERS)

# AES
AES_OBJS := $(addprefix objs/crypto/aes/, aes.o aes_c.o aes_ni.o)
AES_HEADERS := $(addprefix src/crypto/aes/, aes.h)
$(AES_OBJS): $(AES_HEADERS)

# COMMON
COMMON_OBJS := $(addprefix objs/common/, common.o)
COMMON_HEADERS := $(addprefix src/common/, common.h)
$(COMMON_OBJS): $(COMMON_HEADERS)

# SHA3
SHA3_OBJS := $(addprefix objs/crypto/sha3/, sha3.o)
SHA3_HEADERS := $(addprefix src/crypto/sha3/, sha3.h)
$(SHA3_OBJS): $(SHA3_HEADERS)

# KEX
objs/kex/kex.o: src/kex/kex.h

# LIB


RAND_OBJS := $(RAND_URANDOM_AESCTR_OBJS) $(RAND_URANDOM_CHACHA_OBJS) objs/crypto/rand/rand.o

KEX_OBJS := $(KEX_RLWE_BCNS15_OBJS) $(KEX_RLWE_NEWHOPE_OBJS) $(KEX_RLWE_MSRLN16_OBJS) $(KEX_LWE_FRODO_OBJS) $(KEX_SIDH_CLN16_OBJS) $(KEX_SIDH_IQC_REF_OBJS) objs/kex/kex.o
#KEX_OBJS := $(KEX_SIDH_IQC_REF_OBJS) objs/kex/kex.o

ifdef ENABLE_CODE_MCBITS
KEX_OBJS += $(KEX_CODE_MCBITS_OBJS)
endif

lib: $(RAND_OBJS) $(KEX_OBJS) $(AES_OBJS) $(COMMON_OBJS) $(SHA3_OBJS)
	@rm -f liboqs.a
	@$(AR) liboqs.a $^
	@$(ECHO) "AR liboqs.a"
	@$(RANLIB) liboqs.a
	@$(ECHO) "RANLIB liboqs.a"

tests: lib src/crypto/rand/test_rand.c src/kex/test_kex.c src/crypto/aes/test_aes.c src/ds_benchmark.h
	@$(CC) $(CFLAGS) $(INCLUDES) -L. src/crypto/rand/test_rand.c -loqs $(LDFLAGS) -o test_rand 
	@$(ECHO) "CC src/crypto/rand/test_rand.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -L. src/kex/test_kex.c -loqs $(LDFLAGS) -o test_kex
	@$(ECHO) "CC src/kex/test_kex.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -L. src/crypto/aes/test_aes.c -loqs $(LDFLAGS) -o test_aes
	@$(ECHO) "CC src/crypto/aes/test_aes.c"

docs: links
	doxygen

check: links tests
	./test_kex --quiet
	./test_rand --quiet
	./test_aes

clean:
	$(RM) -r docs/doxygen objs include
	$(RM) -r test_rand{,.dSYM} test_kex{,.dSYM} test_aes{,.dSYM} liboqs.a
	find . -name .DS_Store -type f -delete

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none src/*.h src/*/*.h src/*/*.c
