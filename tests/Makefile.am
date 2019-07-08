AUTOMAKE_OPTIONS = foreign
check_PROGRAMS = 	example_kem speed_kem test_kem kat_kem \
	example_sig speed_sig test_sig \
	test_aes test_sha3

commonflags  = -L../.libs -loqs -lm
if USE_OPENSSL
commonflags += -L${OPENSSL_DIR}/lib -lcrypto
endif

example_kem_SOURCES     = example_kem.c
example_sig_SOURCES     = example_sig.c
speed_kem_SOURCES       = speed_kem.c
speed_sig_SOURCES       = speed_sig.c
test_aes_SOURCES        = test_aes.c
test_kem_SOURCES        = test_kem.c
test_sha3_SOURCES       = test_sha3.c
test_sig_SOURCES        = test_sig.c
kat_kem_SOURCES         = kat_kem.c

example_kem_LDFLAGS     = ${commonflags}
example_sig_LDFLAGS     = ${commonflags}
kat_kem_LDFLAGS         = ${commonflags}
speed_kem_LDFLAGS       = ${commonflags}
speed_sig_LDFLAGS       = ${commonflags}
test_kem_LDFLAGS        = ${commonflags}
test_sig_LDFLAGS        = ${commonflags}
test_aes_LDFLAGS        = -L../src/crypto/aes/.libs -laes # required since OQS_AES symbols are not part of OQS public API
test_aes_LDFLAGS        += ${commonflags} 
test_sha3_LDFLAGS       = -L../src/crypto/sha3/.libs -lsha3 # required since OQS_SHA3 symbols are not part of OQS public API
test_sha3_LDFLAGS       += ${commonflags}
