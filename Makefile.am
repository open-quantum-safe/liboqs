AUTOMAKE_OPTIONS = foreign 
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = doxygen.cfg

CLANGFORMAT ?= clang-format-3.7

SUBDIRS = ${SRCDIR}

if USE_MCBITS
SUBDIRS += src/kex_code_mcbits/ 
endif

BUILT_SOURCES = links
lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD = src/common/libcommon.la src/kex/libkex.la src/crypto/rand/librand.la src/crypto/aes/libaes.la
liboqs_la_LIBADD += src/crypto/rand_urandom_aesctr/librandaesctr.la src/crypto/sha3/libsha3.la
liboqs_la_LIBADD += src/crypto/rand_urandom_chacha20/librandchacha20.la
liboqs_la_LIBADD += src/kex_rlwe_bcns15/libbcns15.la src/kex_rlwe_newhope/libnewhope.la
liboqs_la_LIBADD += src/kex_lwe_frodo/libfrodo.la src/kex_rlwe_msrln16/libmsrln16.la
liboqs_la_LIBADD += src/kex_sidh_cln16/libcln16.la

if USE_MCBITS
liboqs_la_LIBADD += src/kex_code_mcbits/libmcbits.la
endif

noinst_bin_PROGRAMS = test_rand test_kex test_aes
noinst_bindir=$(prefix)/tests
test_kex_LDADD =  liboqs.la -lm
test_kex_SOURCES = src/kex/test_kex.c
test_kex_CPPFLAGS = -I./include
test_kex_CPPFLAGS +=  $(AM_CPPFLAGS) 
if USE_OPENSSL
test_kex_LDADD += -lcrypto
endif

test_aes_LDADD =  liboqs.la -lm
test_aes_SOURCES = src/crypto/aes/test_aes.c
test_aes_CPPFLAGS = -I./include
test_aes_CPPFLAGS += $(AM_CPPFLAGS)

if USE_AES_NI
test_aes_CPPFLAGS += -DAES_ENABLE_NI
endif


if USE_OPENSSL
test_aes_LDADD += -lcrypto
endif

test_rand_SOURCES = src/crypto/rand/test_rand.c

test_rand_LDADD = liboqs.la
if USE_OPENSSL
test_rand_CPPFLAGS = -DUSE_OPENSSL
test_rand_LDADD += -lcrypto
else
if USE_AES_NI
test_rand_CPPFLAGS = -maes -msse2 -DAES_ENABLE_NI
else
test_rand_CPPFLAGS = -DAES_DISABLE_NI
endif
endif

test_rand_CPPFLAGS += -Iinclude -Isrc/crypto/rand_urandom_aesctr/
test_rand_CPPFLAGS += $(AM_CPPFLAGS)

test:
	$(foreach s,$(ALLDIRS),make test -s -C $(s);)
	./test_kex
	./test_rand
	./test_aes

links:
	$(MKDIR_P) include/oqs
	$(LN_S) -f ../../src/common/common.h include/oqs
	$(LN_S) -f ../../src/crypto/aes/aes.h include/oqs
	$(LN_S) -f ../../src/crypto/sha3/sha3.h include/oqs
	$(LN_S) -f ../../src/kex/kex.h include/oqs
	$(LN_S) -f ../../src/crypto/rand/rand.h include/oqs
	$(LN_S) -f ../../src/crypto/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs
	$(LN_S) -f ../../src/crypto/rand_urandom_aesctr/rand_urandom_aesctr.h include/oqs
	$(LN_S) -f ../../src/kex_rlwe_newhope/kex_rlwe_newhope.h include/oqs
	$(LN_S) -f ../../src/kex_rlwe_msrln16/kex_rlwe_msrln16.h include/oqs
	$(LN_S) -f ../../src/kex_lwe_frodo/kex_lwe_frodo.h include/oqs
	$(LN_S) -f ../../src/kex_rlwe_bcns15/kex_rlwe_bcns15.h include/oqs
	$(LN_S) -f ../../src/kex_sidh_cln16/kex_sidh_cln16.h include/oqs
if USE_MCBITS
	$(LN_S) -f ../../src/kex_code_mcbits/kex_code_mcbits.h include/oqs
endif

clean-local:
	rm -rf docs

prettyprint:
	find src -name '*.c' -o -name '*.h' | xargs $(CLANGFORMAT) -style=file -i

docs: links
	doxygen

