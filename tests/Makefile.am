AUTOMAKE_OPTIONS = foreign
check_PROGRAMS = 	example_kem speed_kem test_kem kat_kem \
	example_sig speed_sig test_sig kat_sig \
	test_aes test_sha3 test_hash

commonldflags  = -L../.libs -loqs -lm
if USE_OPENSSL
commonldflags += -L${OPENSSL_DIR}/lib -lcrypto
endif

git_commit=$(shell ../scripts/git_commit.sh)
CFLAGS += '-DOQS_COMPILE_GIT_COMMIT="${git_commit}"'

example_kem_SOURCES     = example_kem.c
example_sig_SOURCES     = example_sig.c
speed_kem_SOURCES       = speed_kem.c
speed_sig_SOURCES       = speed_sig.c
test_aes_SOURCES        = test_aes.c
test_hash_SOURCES       = test_hash.c
test_kem_SOURCES        = test_kem.c
test_sha3_SOURCES       = test_sha3.c
test_sig_SOURCES        = test_sig.c
kat_kem_SOURCES         = kat_kem.c
kat_sig_SOURCES         = kat_sig.c

example_kem_LDFLAGS     = ${commonldflags}
example_sig_LDFLAGS     = ${commonldflags}
kat_kem_LDFLAGS         = ${commonldflags}
kat_sig_LDFLAGS         = ${commonldflags}
speed_kem_LDFLAGS       = ${commonldflags}
speed_sig_LDFLAGS       = ${commonldflags}
test_kem_LDFLAGS        = ${commonldflags}
test_sig_LDFLAGS        = ${commonldflags}
test_aes_LDFLAGS        = -L../src/crypto/aes/.libs -laes # required since OQS_AES symbols are not part of OQS public API
test_aes_LDFLAGS        += ${commonldflags} 
test_hash_LDFLAGS       = -L../src/crypto/sha2/.libs -lsha2 # required since OQS_SHA2 symbols are not part of OQS public API
test_hash_LDFLAGS       += -L../src/crypto/sha3/.libs -lsha3 # required since OQS_SHA3 symbols are not part of OQS public API
test_hash_LDFLAGS       += ${commonldflags}
test_sha3_LDFLAGS       = -L../src/crypto/sha3/.libs -lsha3 # required since OQS_SHA3 symbols are not part of OQS public API
test_sha3_LDFLAGS       += ${commonldflags}

example_kem_DEPENDENCIES = ../liboqs.la
example_sig_DEPENDENCIES = ../liboqs.la
speed_kem_DEPENDENCIES   = ../liboqs.la
speed_sig_DEPENDENCIES   = ../liboqs.la
test_kem_DEPENDENCIES    = ../liboqs.la
test_sig_DEPENDENCIES    = ../liboqs.la
test_aes_DEPENDENCIES    = ../liboqs.la
test_hash_DEPENDENCIES   = ../liboqs.la
test_sha3_DEPENDENCIES   = ../liboqs.la
