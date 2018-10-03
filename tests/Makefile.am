AUTOMAKE_OPTIONS = foreign
if ENABLESHARED
check_PROGRAMS = example_kem
else
check_PROGRAMS = 	example_kem speed_kem test_kem \
	minimal_kex_oqs test_kex \
	test_sig \
	test_aes test_rand test_sha3 kat_kem

if ENABLE_SIG_PICNIC
check_PROGRAMS += minimal_sig_oqs
endif
endif

LIB_FLAGS=../liboqs.la -lm -lcrypto

example_kem_SOURCES     = example_kem.c
speed_kem_SOURCES       = speed_kem.c
minimal_kex_oqs_SOURCES = minimal_kex_oqs.c
if ENABLE_SIG_PICNIC
minimal_sig_oqs_SOURCES = minimal_sig_oqs.c
endif
test_aes_SOURCES        = test_aes.c
test_kem_SOURCES        = test_kem.c
test_kex_SOURCES        = test_kex.c
test_rand_SOURCES       = test_rand.c
test_sha3_SOURCES       = test_sha3.c
test_sig_SOURCES        = test_sig.c
kat_kem_SOURCES         = kat_kem.c

example_kem_LDADD     = ${LIB_FLAGS}
minimal_kex_oqs_LDADD = ${LIB_FLAGS}

if ENABLE_SIG_PICNIC
minimal_sig_oqs_LDADD = ${LIB_FLAGS}
endif
speed_kem_LDADD       = ${LIB_FLAGS}
test_aes_LDADD        = ${LIB_FLAGS}
test_kem_LDADD        = ${LIB_FLAGS}
test_kex_LDADD        = ${LIB_FLAGS}
test_rand_LDADD       = ${LIB_FLAGS}
test_sha3_LDADD       = ${LIB_FLAGS}
test_sig_LDADD        = ${LIB_FLAGS}
kat_kem_LDADD         = ${LIB_FLAGS}

if USE_OPENSSL
example_kem_LDADD     += -L$(OPENSSL_DIR)/lib -lcrypto
if ENABLE_SIG_PICNIC
minimal_sig_oqs_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
endif
minimal_kex_oqs_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
speed_kem_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_aes_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_kem_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_kex_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_rand_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_sha3_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_sig_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
endif
