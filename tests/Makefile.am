AUTOMAKE_OPTIONS = foreign
check_PROGRAMS = example_kem speed_kem test_kem \
                      minimal_kex_oqs test_kex \
                      example_sig test_sig \
                      test_aes test_rand test_sha3 kat_kem

example_kem_SOURCES     = example_kem.c
example_sig_SOURCES     = example_sig.c
speed_kem_SOURCES       = speed_kem.c
minimal_kex_oqs_SOURCES = minimal_kex_oqs.c
test_aes_SOURCES        = test_aes.c
test_kem_SOURCES        = test_kem.c
test_kex_SOURCES        = test_kex.c
test_rand_SOURCES       = test_rand.c
test_sha3_SOURCES       = test_sha3.c
test_sig_SOURCES        = test_sig.c
kat_kem_SOURCES 	= kat_kem.c

example_kem_LDADD     = -L.. -loqs -lm -lcrypto
example_sig_LDADD     = -L.. -loqs -lm -lcrypto
minimal_kex_oqs_LDADD = -L.. -loqs -lm -lcrypto
speed_kem_LDADD       = -L.. -loqs -lm -lcrypto
test_aes_LDADD        = -L.. -loqs -lm -lcrypto
test_kem_LDADD        = -L.. -loqs -lm -lcrypto
test_kex_LDADD        = -L.. -loqs -lm -lcrypto
test_rand_LDADD       = -L.. -loqs -lm -lcrypto
test_sha3_LDADD       = -L.. -loqs -lm -lcrypto
test_sig_LDADD        = -L.. -loqs -lm -lcrypto
kat_kem_LDADD         = ../liboqs.la -lm -lcrypto

if USE_OPENSSL
example_kem_LDADD     += -L$(OPENSSL_DIR)/lib -lcrypto
example_sig_LDADD     += -L$(OPENSSL_DIR)/lib -lcrypto
minimal_kex_oqs_LDADD += -L$(OPENSSL_DIR)/lib -lcrypto
speed_kem_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_aes_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_kem_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_kex_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
test_rand_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_sha3_LDADD       += -L$(OPENSSL_DIR)/lib -lcrypto
test_sig_LDADD        += -L$(OPENSSL_DIR)/lib -lcrypto
endif
