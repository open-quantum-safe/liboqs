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
liboqs_la_LIBADD += src/crypto/sha2/libsha2.la
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
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_IF_ENABLE_START
if ENABLE_KEM_KYBER
liboqs_la_LIBADD += src/kem/kyber/libkemkyber.la
endif
if ENABLE_KEM_NEWHOPE
liboqs_la_LIBADD += src/kem/newhope/libkemnewhope.la
endif
if ENABLE_KEM_NTRU
liboqs_la_LIBADD += src/kem/ntru/libkemntru.la
endif
if ENABLE_KEM_SABER
liboqs_la_LIBADD += src/kem/saber/libkemsaber.la
endif

if ENABLE_SIG_DILITHIUM
liboqs_la_LIBADD += src/sig/dilithium/libsigdilithium.la
endif
if ENABLE_SIG_MQDSS
liboqs_la_LIBADD += src/sig/mqdss/libsigmqdss.la
endif
if ENABLE_SIG_RAINBOW
liboqs_la_LIBADD += src/sig/rainbow/libsigrainbow.la
endif
if ENABLE_SIG_SPHINCS
liboqs_la_LIBADD += src/sig/sphincs/libsigsphincs.la
endif
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_IF_ENABLE_END
if ENABLE_KEM_FRODOKEM
liboqs_la_LIBADD += src/kem/frodokem/libkemfrodokem.la
endif
if ENABLE_KEM_SIKE
liboqs_la_LIBADD += src/kem/sike/libkemsike.la
endif

if USE_OPENSSL
liboqs_la_LIBADD += -L${OPENSSL_DIR}/lib -lcrypto
endif

installheaderdir=$(includedir)/oqs
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_INSTALLHEADER_START
installheader_HEADERS= src/oqs.h \
	src/oqsconfig.h \
	src/common/common.h \
	src/common/rand.h \
	src/crypto/aes/aes.h \
	src/crypto/sha2/sha2.h \
	src/crypto/sha3/sha3.h \
	src/kem/kem.h \
	src/kem/bike/kem_bike.h \
	src/kem/kyber/kem_kyber.h \
	src/kem/newhope/kem_newhope.h \
	src/kem/ntru/kem_ntru.h \
	src/kem/saber/kem_saber.h \
	src/kem/frodokem/kem_frodokem.h \
	src/kem/sike/kem_sike.h \
	src/sig/sig.h \
	src/sig/dilithium/sig_dilithium.h \
	src/sig/mqdss/sig_mqdss.h \
	src/sig/rainbow/sig_rainbow.h \
	src/sig/sphincs/sig_sphincs.h \
	src/sig/picnic/sig_picnic.h \
	src/sig/qtesla/sig_qtesla.h
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_INSTALLHEADER_END


test: check
	tests/example_kem
	tests/example_sig
	tests/test_kem
	tests/test_sig
	tests/test_aes
	tests/test_sha3
	python3 -m pytest -v

kat: clean-kats check
	tests/kat_kem
	scripts/check_kats.sh

links:
	$(MKDIR_P) include/oqs
	cp -f src/oqs.h include/oqs
	grep OQS_ config.h > src/oqsconfig.h
	grep USE_ config.h >> src/oqsconfig.h
	cp -f src/oqsconfig.h include/oqs
	cp -f src/common/common.h include/oqs
	cp -f src/common/rand.h include/oqs
	cp -f src/crypto/aes/aes.h include/oqs
	cp -f src/crypto/sha2/sha2.h include/oqs
	cp -f src/crypto/sha3/sha3.h include/oqs
	cp -f src/kem/kem.h include/oqs
	cp -f src/kem/bike/kem_bike.h include/oqs
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_LINKS_START
	cp -f src/kem/kyber/kem_kyber.h include/oqs
	cp -f src/kem/newhope/kem_newhope.h include/oqs
	cp -f src/kem/ntru/kem_ntru.h include/oqs
	cp -f src/kem/saber/kem_saber.h include/oqs
	cp -f src/sig/dilithium/sig_dilithium.h include/oqs
	cp -f src/sig/mqdss/sig_mqdss.h include/oqs
	cp -f src/sig/rainbow/sig_rainbow.h include/oqs
	cp -f src/sig/sphincs/sig_sphincs.h include/oqs
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_LINKS_END
	cp -f src/kem/frodokem/kem_frodokem.h include/oqs
	cp -f src/kem/sike/kem_sike.h include/oqs
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
	find src tests -name '*.c' -o -name '*.h' | grep -v picnic/external* | grep -v frodokem/external* | grep -v pqclean | xargs $(CLANGFORMAT) -style=file -i

docs: links doxygen-run
