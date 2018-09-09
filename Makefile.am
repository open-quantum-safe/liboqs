AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = docs/.Doxyfile

CLANGFORMAT ?= clang-format-3.9

SUBDIRS = ${SRCDIR} . tests

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
if ENABLE_KEX_NTRU
liboqs_la_LIBADD += src/kex_ntru/libntru.la
endif
if ENABLE_KEX_SIDH_MSR
liboqs_la_LIBADD += src/kex_sidh_msr/libsidhmsr.la
endif

liboqs_la_LIBADD += src/sig/libsig.la

if ENABLE_MCBITS
liboqs_la_LIBADD += src/kex_code_mcbits/libmcbits.la
endif

if ENABLE_SIG_PICNIC
liboqs_la_LIBADD += src/sig_picnic/libpicnic_i.la
endif
if ENABLE_SIG_QTESLA
liboqs_la_LIBADD += src/sig_qtesla/libqtesla.la
endif

if ENABLE_KEM_BIKE
liboqs_la_LIBADD += src/kem/bike/libkembike.la
endif
if ENABLE_KEM_FRODOKEM
liboqs_la_LIBADD += src/kem/frodokem/libkemfrodokem.la
endif
if ENABLE_KEM_SIKE
liboqs_la_LIBADD += src/kem/sike/libkemsike.la
endif
if ENABLE_KEM_NEWHOPE
liboqs_la_LIBADD += src/kem/newhope/libkemnewhope.la
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
	src/kex_sidh_msr/kex_sidh_msr.h \
	src/kem/bike/kem_bike.h \
	src/kem/frodokem/kem_frodokem.h \
	src/kem/sike/kem_sike.h \
	src/kem/newhope/kem_newhope.h \
	src/sig/sig.h

test: check
	tests/example_kem
	tests/test_kem
	tests/test_kex --quiet
	tests/test_rand --quiet
	tests/test_aes
	tests/test_sha3
	tests/test_sig
if ENABLE_SIG_PICNIC
	tests/minimal_sig_oqs
endif

kat: clean-kats
	make
	tests/kat_kem
	scripts/check_kats.sh

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
	cp -f src/kem/bike/kem_bike.h include/oqs
	cp -f src/kem/frodokem/kem_frodokem.h include/oqs
	cp -f src/kem/sike/kem_sike.h include/oqs
	cp -f src/kem/newhope/kem_newhope.h include/oqs
	cp -f src/kex/kex.h include/oqs
	cp -f src/kex_ntru/kex_ntru.h include/oqs
	cp -f src/kex_sidh_msr/kex_sidh_msr.h include/oqs
	cp -f src/sig/sig.h include/oqs
	cp -f src/sig_picnic/sig_picnic.h include/oqs
	cp -f src/sig_qtesla/sig_qtesla.h include/oqs
if ENABLE_MCBITS
	cp -f src/kex_code_mcbits/kex_code_mcbits.h include/oqs
endif

clean-local:
	rm -f liboqs.a
	rm -rf include

clean-tests:
	rm -f tests/example_kem tests/speed_kem tests/test_kem tests/minimal_kex_oqs tests/test_kex tests/minimal_sig_oqs tests/test_sig tests/test_rand tests/test_aes tests/test_sha3

clean-kats:
	rm -rf tests/kat_kem kat_kem_rsp/

prettyprint:
	find src tests -name '*.c' -o -name '*.h' | grep -v "sig_picnic/external*" | grep -v "kex_sidh_msr" | xargs $(CLANGFORMAT) -style=file -i

docs: doxygen-run
