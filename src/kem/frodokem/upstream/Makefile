####  Makefile for compilation on Unix-like operative systems  ####

CC=gcc
ifeq "$(CC)" "gcc"
    COMPILER=gcc
else ifeq "$(CC)" "clang"
    COMPILER=clang
endif

ARCHITECTURE=_AMD64_
ifeq "$(ARCH)" "x64"
    ARCHITECTURE=_AMD64_
else ifeq "$(ARCH)" "x86"
    ARCHITECTURE=_X86_
    USE_OPT_LEVEL=_FAST_GENERIC_
else ifeq "$(ARCH)" "ARM"
    ARCHITECTURE=_ARM_
    USE_OPT_LEVEL=_FAST_GENERIC_
endif

ifeq "$(ARCHITECTURE)" "_AMD64_"
    USE_OPT_LEVEL=_FAST_
endif

ifeq "$(OPT_LEVEL)" "REFERENCE"
    USE_OPT_LEVEL=_REFERENCE_
else ifeq "$(OPT_LEVEL)" "FAST_GENERIC"
    USE_OPT_LEVEL=_FAST_GENERIC_
else ifeq "$(OPT_LEVEL)" "FAST"
    ifeq "$(ARCHITECTURE)" "_AMD64_"
        USE_OPT_LEVEL=_FAST_
    endif
endif

USE_GENERATION_A=_AES128_FOR_A_
ifeq "$(GENERATION_A)" "AES128_ECB"
    USE_GENERATION_A=_AES128_FOR_A_
else ifeq "$(GENERATION_A)" "CSHAKE128"
    USE_GENERATION_A=_CSHAKE128_FOR_A_
endif

ifeq "$(ARCH)" "ARM"
    ARM_SETTING=-lrt
endif

USING_OPENSSL=_USE_OPENSSL_
ifeq "$(USE_OPENSSL)" "FALSE"
    USING_OPENSSL=NO_OPENSSL
endif

OPENSSL_INCLUDE_DIR=/usr/include
OPENSSL_LIB_DIR=/usr/lib

AR=ar rcs
RANLIB=ranlib
LN=ln -s

CFLAGS= -O3 -std=gnu11 -Wall -Wextra -march=native -DNIX -D $(ARCHITECTURE) -D $(USE_OPT_LEVEL) -D $(USE_GENERATION_A) -D $(USING_OPENSSL)
ifeq "$(USE_OPENSSL)" "FALSE"
LDFLAGS=-lm
else
CFLAGS+= -I$(OPENSSL_INCLUDE_DIR)
LDFLAGS=-lm -L$(OPENSSL_LIB_DIR) -lssl -lcrypto
endif

ifeq "$(ARCHITECTURE)" "_AMD64_"
ifeq "$(USE_OPT_LEVEL)" "_FAST_"
CFLAGS += -mavx2 -maes -msse2
endif
endif

.PHONY: all check clean prettyprint

all: lib640 lib976 tests KATS

objs/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -c  $(CFLAGS) $< -o $@


objs/frodo640.o: src/frodo640.c
	@mkdir -p $(@D)
	$(CC) -c  $(CFLAGS) $< -o $@

objs/frodo976.o: src/frodo976.c
	@mkdir -p $(@D)
	$(CC) -c  $(CFLAGS) $< -o $@


# RAND
objs/random/random.o: src/random/random.h
RAND_OBJS := objs/random/random.o

# KEM_FRODO
KEM_FRODO640_OBJS := $(addprefix objs/, frodo640.o util.o)
KEM_FRODO640_HEADERS := $(addprefix src/, api_frodo640.h config.h frodo_macrify.h)
$(KEM_FRODO640_OBJS): $(KEM_FRODO640_HEADERS)

KEM_FRODO976_OBJS := $(addprefix objs/, frodo976.o util.o)
KEM_FRODO976_HEADERS := $(addprefix src/, api_frodo976.h config.h frodo_macrify.h)
$(KEM_FRODO976_OBJS): $(KEM_FRODO976_HEADERS)

# AES
AES_OBJS := $(addprefix objs/aes/, aes.o aes_c.o)
AES_HEADERS := $(addprefix src/aes/, aes.h)
$(AES_OBJS): $(AES_HEADERS)

# SHAKE
SHAKE_OBJS := $(addprefix objs/sha3/, fips202.o)
SHAKE_HEADERS := $(addprefix src/sha3/, fips202.h)
$(SHAKE_OBJS): $(SHAKE_HEADERS)

ifeq "$(USE_OPT_LEVEL)" "_FAST_"
# AES_NI
AES_NI_OBJS := $(addprefix objs/aes/, aes_ni.o)

ifeq "$(GENERATION_A)" "CSHAKE128"
# SHAKEx4
SHAKEx4_OBJS := $(addprefix objs/sha3/, fips202x4.o keccak4x/KeccakP-1600-times4-SIMD256.o)
SHAKEx4_HEADERS := $(addprefix src/sha3/, fips202x4.h keccak4x/KeccakP-1600-times4-SnP.h)
$(SHAKEx4_OBJS): $(SHAKEx4_HEADERS)
endif
endif

lib640: $(KEM_FRODO640_OBJS) $(RAND_OBJS) $(AES_OBJS) $(AES_NI_OBJS) $(SHAKE_OBJS) $(SHAKEx4_OBJS)
	rm -rf frodo640
	mkdir frodo640
	$(AR) frodo640/libfrodo.a $^
	$(RANLIB) frodo640/libfrodo.a

lib976: $(KEM_FRODO976_OBJS) $(RAND_OBJS) $(AES_OBJS) $(AES_NI_OBJS) $(SHAKE_OBJS) $(SHAKEx4_OBJS)
	rm -rf frodo976
	mkdir frodo976
	$(AR) frodo976/libfrodo.a $^
	$(RANLIB) frodo976/libfrodo.a

tests: lib640 lib976 tests/ds_benchmark.h
	$(CC) $(CFLAGS) -L./frodo640 tests/test_KEM640.c -lfrodo $(LDFLAGS) -o frodo640/test_KEM $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./frodo976 tests/test_KEM976.c -lfrodo $(LDFLAGS) -o frodo976/test_KEM $(ARM_SETTING)

lib640_for_KATs: $(KEM_FRODO640_OBJS) $(AES_OBJS) $(AES_NI_OBJS) $(SHAKE_OBJS) $(SHAKEx4_OBJS)
	$(AR) frodo640/libfrodo_for_testing.a $^
	$(RANLIB) frodo640/libfrodo_for_testing.a

lib976_for_KATs: $(KEM_FRODO976_OBJS) $(AES_OBJS) $(AES_NI_OBJS) $(SHAKE_OBJS) $(SHAKEx4_OBJS)
	$(AR) frodo976/libfrodo_for_testing.a $^
	$(RANLIB) frodo976/libfrodo_for_testing.a

KATS: lib640_for_KATs lib976_for_KATs
ifeq "$(GENERATION_A)" "CSHAKE128"
	$(CC) $(CFLAGS) -L./frodo640 tests/PQCtestKAT_kem640_cshake.c tests/rng.c -lfrodo_for_testing $(LDFLAGS) -o frodo640/PQCtestKAT_kem_cshake $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./frodo976 tests/PQCtestKAT_kem976_cshake.c tests/rng.c -lfrodo_for_testing $(LDFLAGS) -o frodo976/PQCtestKAT_kem_cshake $(ARM_SETTING)
else
	$(CC) $(CFLAGS) -L./frodo640 tests/PQCtestKAT_kem640.c tests/rng.c -lfrodo_for_testing $(LDFLAGS) -o frodo640/PQCtestKAT_kem $(ARM_SETTING)
	$(CC) $(CFLAGS) -L./frodo976 tests/PQCtestKAT_kem976.c tests/rng.c -lfrodo_for_testing $(LDFLAGS) -o frodo976/PQCtestKAT_kem $(ARM_SETTING)
endif

check: tests

clean:
	rm -rf objs *.req frodo640 frodo976
	find . -name .DS_Store -type f -delete

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none src/*.h src/*/*.h src/*/*.c
