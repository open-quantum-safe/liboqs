AUTOMAKE_OPTIONS = foreign
check_PROGRAMS = 	example_kem speed_kem test_kem kat_kem \
	example_sig speed_sig test_sig
if !ENABLE_SHARED
check_PROGRAMS += test_aes test_sha3
endif

LIB_FLAGS=../liboqs.la -lm -lcrypto

example_kem_SOURCES     = example_kem.c
example_sig_SOURCES     = example_sig.c
speed_kem_SOURCES       = speed_kem.c
speed_sig_SOURCES       = speed_sig.c
test_aes_SOURCES        = test_aes.c
test_kem_SOURCES        = test_kem.c
test_sha3_SOURCES       = test_sha3.c
test_sig_SOURCES        = test_sig.c
kat_kem_SOURCES         = kat_kem.c

example_kem_LDADD     = ${LIB_FLAGS}
example_sig_LDADD     = ${LIB_FLAGS}
speed_kem_LDADD       = ${LIB_FLAGS}
speed_sig_LDADD       = ${LIB_FLAGS}
test_aes_LDADD        = ${LIB_FLAGS}
test_kem_LDADD        = ${LIB_FLAGS}
test_sha3_LDADD       = ${LIB_FLAGS}
test_sig_LDADD        = ${LIB_FLAGS}
kat_kem_LDADD         = ${LIB_FLAGS}

if USE_OPENSSL
example_kem_LDADD     += -L$(OPENSSL_DIR)/lib -lcrypto
example_sig_LDADD     += -L$(OPENSSL_DIR)/lib -lcrypto
speed_kem_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
speed_sig_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_aes_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_kem_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_sha3_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_sig_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
kat_kem_LDADD         += -L$(OPENSSL_DIR)/lib -lcrypto
endif
