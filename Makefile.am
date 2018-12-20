AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = docs/.Doxyfile

CLANGFORMAT ?= clang-format-3.9

SUBDIRS = ${SRCDIR} . tests

BUILT_SOURCES = oqsconfigh links
lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD =  src/common/libcommon.la
liboqs_la_LIBADD += src/crypto/aes/libaes.la
liboqs_la_LIBADD += src/crypto/sha3/libsha3.la
liboqs_la_LIBADD += src/kem/libkem.la
liboqs_la_LIBADD += src/sig/libsig.la

if ENABLE_SIG_PICNIC
liboqs_la_LIBADD += src/sig/picnic/libpicnic_i.la
endif
if ENABLE_SIG_QTESLA
liboqs_la_LIBADD += src/sig/qtesla/libqtesla.la
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
liboqs_la_LIBADD += src/kem/newhopenist/libkemnewhopenist.la
endif
if ENABLE_KEM_KYBER
liboqs_la_LIBADD += src/kem/kyber/libkemkyber.la
endif

oqsconfigh:
	grep OQS_ config.h > src/oqsconfig.h

installheaderdir=$(includedir)/oqs
installheader_HEADERS= src/oqs.h \
	src/oqsconfig.h \
	src/common/common.h \
	src/common/rand.h \
	src/crypto/aes/aes.h \
	src/crypto/sha3/sha3.h \
	src/kem/kem.h \
	src/kem/bike/kem_bike.h \
	src/kem/frodokem/kem_frodokem.h \
	src/kem/newhopenist/kem_newhopenist.h \
	src/kem/kyber/kem_kyber.h \
	src/kem/sike/kem_sike.h \
	src/sig/sig.h \
	src/sig/picnic/sig_picnic.h \
	src/sig/qtesla/sig_qtesla.h


test: clean-tests check
	tests/example_kem
	tests/example_sig
	tests/test_kem
	tests/test_sig
if !ENABLE_SHARED
	tests/test_aes
	tests/test_sha3
endif

kat: clean-kats check
	tests/kat_kem
	scripts/check_kats.sh

links: oqsconfigh
	$(MKDIR_P) include/oqs
	cp -f src/oqs.h include/oqs
	cp -f src/oqsconfig.h include/oqs
	cp -f src/common/common.h include/oqs
	cp -f src/common/rand.h include/oqs
	cp -f src/crypto/aes/aes.h include/oqs
	cp -f src/crypto/sha3/sha3.h include/oqs
	cp -f src/kem/kem.h include/oqs
	cp -f src/kem/bike/kem_bike.h include/oqs
	cp -f src/kem/frodokem/kem_frodokem.h include/oqs
	cp -f src/kem/sike/kem_sike.h include/oqs
	cp -f src/kem/newhopenist/kem_newhopenist.h include/oqs
	cp -f src/kem/kyber/kem_kyber.h include/oqs
	cp -f src/sig/sig.h include/oqs
	cp -f src/sig/picnic/sig_picnic.h include/oqs
	cp -f src/sig/qtesla/sig_qtesla.h include/oqs

clean-local:
	rm -rf include

clean-tests:
	rm -f tests/example_kem tests/example_sig tests/speed_kem tests/speed_sig tests/test_kem tests/test_sig tests/test_aes tests/test_sha3

clean-kats:
	rm -rf tests/kat_kem kat_kem_rsp/

prettyprint:
	find src tests -name '*.c' -o -name '*.h' | grep -v picnic/external* | xargs $(CLANGFORMAT) -style=file -i

docs: links doxygen-run
