AUTOMAKE_OPTIONS = foreign 
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = doxygen.cfg

SUBDIRS = ${SRCDIR}

BUILT_SOURCES = link
lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD = src/common/libcommon.la src/kex/libkex.la src/crypto/rand/librand.la src/crypto/aes/libaes.la
liboqs_la_LIBADD += src/crypto/rand_urandom_aesctr/librandaesctr.la src/crypto/sha3/libsha3.la
liboqs_la_LIBADD += src/crypto/rand_urandom_chacha20/librandchacha20.la
liboqs_la_LIBADD += src/kex_rlwe_bcns15/libbcns15.la src/kex_rlwe_newhope/libnewhope.la
liboqs_la_LIBADD += src/kex_lwe_frodo/libfrodo.la src/kex_rlwe_msrln16/libmsrln16.la

noinst_bin_PROGRAMS = test_rand test_kex oqs_bench_kex
noinst_bindir=$(prefix)/tests
test_kex_LDADD =  liboqs.la -lm
test_kex_SOURCES = src/tests/test_kex.c
test_kex_CPPFLAGS = -I./include
test_kex_CPPFLAGS +=  $(AM_CPPFLAGS) 
if USE_OPENSSL
test_kex_LDADD += -lcrypto
endif

oqs_bench_kex_LDADD =  liboqs.la -lm
oqs_bench_kex_SOURCES = src/tests/oqs_bench_kex.c
oqs_bench_kex_CPPFLAGS = -I./include
oqs_bench_kex_CPPFLAGS +=  $(AM_CPPFLAGS) 
if USE_OPENSSL
oqs_bench_kex_LDADD += -lcrypto
endif

test_rand_SOURCES = src/tests/test_rand.c

test_rand_LDADD = liboqs.la
if USE_OPENSSL
test_rand_CPPFLAGS = -DUSE_OPENSSL
test_rand_LDADD += -lcrypto
else
if USE_AES_NI
test_rand_CPPFLAGS = -maes -msse2
else
test_rand_CPPFLAGS = -DAES_DISABLE_NI
endif
endif

test_rand_CPPFLAGS += -Iinclude -Isrc/rand_urandom_aesctr/
test_rand_CPPFLAGS += $(AM_CPPFLAGS)

test:
	$(foreach s,$(ALLDIRS),make test -s -C $(s);)
	./test_kex
	./test_rand
	./oqs_bench_kex

link:
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


prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none src/*/*.h src/*/*.c

