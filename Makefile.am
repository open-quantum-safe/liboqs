AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = docs/.Doxyfile

CLANGFORMAT ?= clang-format-3.9

SUBDIRS = ${SRCDIR}

BUILT_SOURCES = links
lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD =  src/common/libcommon.la
liboqs_la_LIBADD += src/crypto/aes/libaes.la
liboqs_la_LIBADD += src/crypto/rand_urandom_aesctr/librandaesctr.la
liboqs_la_LIBADD += src/crypto/rand_urandom_chacha20/librandchacha20.la
liboqs_la_LIBADD += src/crypto/sha3/libsha3.la
liboqs_la_LIBADD += src/kem/libkem.la
liboqs_la_LIBADD += src/kex/libkex.la
if USE_KEX_RLWE_NEWHOPE_AVX2
liboqs_la_LIBADD += src/kex_rlwe_newhope/avx2/libnewhope_avx2.la
endif
if USE_KEX_NTRU
liboqs_la_LIBADD += src/kex_ntru/libntru.la
endif
if USE_KEX_RLWE_NEWHOPE
liboqs_la_LIBADD += src/kex_rlwe_newhope/libnewhope.la
endif
if USE_KEX_SIDH_MSR
liboqs_la_LIBADD += src/kex_sidh_msr/libsidhmsr.la
endif

liboqs_la_LIBADD += src/sig/libsig.la

if USE_MCBITS
liboqs_la_LIBADD += src/kex_code_mcbits/libmcbits.la
endif

if USE_SIG_PICNIC
liboqs_la_LIBADD += src/sig_picnic/libpicnic_i.la
endif
if USE_SIG_QTESLA
liboqs_la_LIBADD += src/sig_qtesla/libqtesla.la
endif

if ENABLE_KEM_SIKE
liboqs_la_LIBADD += src/kem/sike/libkemsike.la
endif
if ENABLE_KEM_FRODOKEM
liboqs_la_LIBADD += src/kem/frodokem/libkemfrodokem.la
endif

installheaderdir=$(includedir)/oqs
installheader_HEADERS=config.h \
	src/oqs.h \
	src/common/common.h \
	src/common/rand.h \
	src/crypto/aes/aes.h \
	src/crypto/rand_urandom_aesctr/rand_urandom_aesctr.h \
	src/crypto/rand_urandom_chacha20/rand_urandom_chacha20.h \
	src/crypto/sha3/sha3.h \
	src/kem/kem.h \
	src/kex/kex.h \
	src/kex_ntru/kex_ntru.h \
	src/kex_rlwe_newhope/kex_rlwe_newhope.h \
	src/kex_sidh_msr/kex_sidh_msr.h \
	src/kem/sike/kem_sike.h \
	src/kem/frodokem/kem_frodokem.h \
	src/sig/sig.h


noinst_bin_PROGRAMS = example_kem speed_kem test_kem test_kex test_sig test_aes test_rand test_sha3

if USE_SIG_PICNIC
noinst_bin_PROGRAMS += minimal_sig_oqs
endif

noinst_bindir=$(prefix)/tests

speed_kem_LDADD =  liboqs.la -lm
speed_kem_SOURCES = src/kem/speed_kem.c
speed_kem_CPPFLAGS = -I./include
speed_kem_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
speed_kem_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

test_kem_LDADD =  liboqs.la -lm
test_kem_SOURCES = src/kem/test_kem.c
test_kem_CPPFLAGS = -I./include
test_kem_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
test_kem_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

example_kem_LDADD =  liboqs.la -lm
example_kem_SOURCES = src/kem/example_kem.c
example_kem_CPPFLAGS = -I./include
example_kem_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
example_kem_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

test_kex_LDADD =  liboqs.la -lm
test_kex_SOURCES = src/kex/test_kex.c
test_kex_CPPFLAGS = -I./include
test_kex_CPPFLAGS += $(AM_CPPFLAGS)
if USE_MCBITS
test_kex_LDADD += -L${SODIUM_DIR}/lib -lsodium
endif
if USE_OPENSSL
test_kex_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

test_sig_LDADD =  liboqs.la -lm
test_sig_SOURCES = src/sig/test_sig.c
test_sig_CPPFLAGS = -I./include
test_sig_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
test_sig_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

if USE_SIG_PICNIC
minimal_sig_oqs_LDADD =  liboqs.la -lm
minimal_sig_oqs_SOURCES = src/sig/minimal_sig_oqs.c
minimal_sig_oqs_CPPFLAGS = -I./include
minimal_sig_oqs_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
minimal_sig_oqs_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif
endif # USE_SIG_PICNIC

test_aes_LDADD =  liboqs.la -lm
test_aes_SOURCES = src/crypto/aes/test_aes.c
test_aes_CPPFLAGS = -I./include
test_aes_CPPFLAGS += $(AM_CPPFLAGS)

if USE_OPENSSL
test_aes_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif

test_rand_SOURCES = src/crypto/rand/test_rand.c

test_rand_CPPFLAGS = -Iinclude -Isrc/crypto/rand_urandom_aesctr/
test_rand_CPPFLAGS += $(AM_CPPFLAGS)
test_rand_LDADD = liboqs.la
if USE_OPENSSL
test_rand_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
else
if USE_AES_NI
test_rand_CPPFLAGS += -maes -msse2
endif
endif # USE_OPENSSL

test_sha3_LDADD =  liboqs.la -lm
test_sha3_SOURCES = src/crypto/sha3/test_sha3.c
test_sha3_CPPFLAGS = -I./include
test_sha3_CPPFLAGS += $(AM_CPPFLAGS)

test: clean-tests
	make
	./example_kem
	./test_kem
	./test_kex --quiet
	./test_rand --quiet
	./test_aes
	./test_sha3
	./test_sig
if USE_SIG_PICNIC
	./minimal_sig_oqs
endif

links:
	$(MKDIR_P) include/oqs
	cp -f config.h include/oqs
	cp -f src/oqs.h include/oqs
	cp -f src/common/common.h include/oqs
	cp -f src/common/rand.h include/oqs
	cp -f src/crypto/aes/aes.h include/oqs
	cp -f src/crypto/rand_urandom_aesctr/rand_urandom_aesctr.h include/oqs
	cp -f src/crypto/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs
	cp -f src/crypto/sha3/sha3.h include/oqs
	cp -f src/kem/kem.h include/oqs
	cp -f src/kem/sike/kem_sike.h include/oqs
	cp -f src/kem/frodokem/kem_frodokem.h include/oqs
	cp -f src/kex/kex.h include/oqs
	cp -f src/kex_ntru/kex_ntru.h include/oqs
	cp -f src/kex_rlwe_newhope/kex_rlwe_newhope.h include/oqs
if USE_KEX_RLWE_NEWHOPE_AVX2
	cp -f src/kex_rlwe_newhope/avx2/kex_rlwe_newhope_avx2.h include/oqs
endif
	cp -f src/kex_sidh_msr/kex_sidh_msr.h include/oqs
	cp -f src/sig/sig.h include/oqs
	cp -f src/sig_picnic/sig_picnic.h include/oqs
	cp -f src/sig_qtesla/sig_qtesla.h include/oqs
	$(LN_S) -f .libs/liboqs.a
if USE_MCBITS
	cp -f src/kex_code_mcbits/kex_code_mcbits.h include/oqs
endif

clean-local:
	rm -f liboqs.a
	rm -rf include
if USE_SIG_PICNIC
	rm -f minimal_sig_oqs
endif

clean-tests:
	rm -f example_kem speed_kem test_kem test_kex test_sig test_rand test_aes test_sha3

prettyprint:
	find src -name '*.c' -o -name '*.h' | grep -v sig_picnic/external* | grep -v "kex_rlwe_newhope/avx2" | grep -v "kex_sidh_msr" | xargs $(CLANGFORMAT) -style=file -i

docs: doxygen-run
