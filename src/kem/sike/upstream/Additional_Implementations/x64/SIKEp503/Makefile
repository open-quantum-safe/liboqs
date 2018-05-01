####  Makefile for compilation on Linux  ####

OPT=-O3     # Optimization option by default

CC=gcc
ifeq "$(CC)" "gcc"
    COMPILER=gcc
else ifeq "$(CC)" "clang"
    COMPILER=clang
endif

ARCHITECTURE=_AMD64_
USE_OPT_LEVEL=_OPTIMIZED_FAST_

ADDITIONAL_SETTINGS=
ifeq "$(SET)" "EXTENDED"
    ADDITIONAL_SETTINGS=-fwrapv -fomit-frame-pointer -march=native
endif

MULX=-D _MULX_
ifeq "$(USE_MULX)" "FALSE"
    MULX=
endif
ADX=-D _ADX_
ifeq "$(USE_ADX)" "FALSE"
    ADX=
endif

AR=ar rcs
RANLIB=ranlib

CFLAGS=$(OPT) $(ADDITIONAL_SETTINGS) -D $(ARCHITECTURE) -D __LINUX__ -D $(USE_OPT_LEVEL) $(MULX) $(ADX)
LDFLAGS=-lm
EXTRA_OBJECTS_503=objs503/fp_x64.o objs503/fp_x64_asm.o
OBJECTS_503=objs503/P503.o $(EXTRA_OBJECTS_503) objs/random.o objs/fips202.o

all: lib503 tests KATS

objs503/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

objs503/fp_x64.o: AMD64/fp_x64.c
	$(CC) -c $(CFLAGS) AMD64/fp_x64.c -o objs503/fp_x64.o

objs503/fp_x64_asm.o: AMD64/fp_x64_asm.S
	$(CC) -c $(CFLAGS) AMD64/fp_x64_asm.S -o objs503/fp_x64_asm.o

objs/random.o: random/random.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) random/random.c -o objs/random.o

objs/fips202.o: sha3/fips202.c
	$(CC) -c $(CFLAGS) sha3/fips202.c -o objs/fips202.o

lib503: $(OBJECTS_503)
	rm -rf sike
	mkdir sike
	$(AR) sike/libsike.a $^
	$(RANLIB) sike/libsike.a

tests: lib503
	$(CC) $(CFLAGS) -L./sike tests/test_SIKEp503.c tests/test_extras.c -lsike $(LDFLAGS) -o sike/test_KEM

# AES
AES_OBJS=objs/aes.o objs/aes_c.o

objs/%.o: tests/aes/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

lib503_for_KATs: $(OBJECTS_503) $(AES_OBJS)
	$(AR) sike/libsike_for_testing.a $^
	$(RANLIB) sike/libsike_for_testing.a

KATS: lib503_for_KATs
	$(CC) $(CFLAGS) -L./sike tests/PQCtestKAT_kem.c tests/rng/rng.c -lsike_for_testing $(LDFLAGS) -o sike/PQCtestKAT_kem

check: tests

.PHONY: clean

clean:
	rm -rf *.req objs503 objs sike

