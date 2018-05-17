####  Makefile for compilation on Linux  ####

OPT=-O3     # Optimization option by default

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
else ifeq "$(ARCH)" "ARM"
    ARCHITECTURE=_ARM_
    ARM_SETTING=-lrt
else ifeq "$(ARCH)" "ARM64"
    ARCHITECTURE=_ARM64_
    ARM_SETTING=-lrt
endif

ADDITIONAL_SETTINGS=
ifeq "$(SET)" "EXTENDED"
    ADDITIONAL_SETTINGS=-fwrapv -fomit-frame-pointer -march=native
endif

USE_OPT_LEVEL=_OPTIMIZED_GENERIC_

AR=ar rcs
RANLIB=ranlib

CFLAGS=-fPIC $(OPT) $(ADDITIONAL_SETTINGS) -D $(ARCHITECTURE) -D __LINUX__ -D $(USE_OPT_LEVEL)
LDFLAGS=-lm
EXTRA_OBJECTS_751=objs751/fp_generic.o
OBJECTS_751=objs751/P751.o $(EXTRA_OBJECTS_751) objs/fips202.o

all: lib751

objs751/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

objs751/fp_generic.o: generic/fp_generic.c
	$(CC) -c $(CFLAGS) generic/fp_generic.c -o objs751/fp_generic.o

objs/fips202.o: sha3/fips202.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) sha3/fips202.c -o objs/fips202.o

lib751: $(OBJECTS_751)
	rm -rf sike
	mkdir sike
	$(AR) sike/libsike.a $^
	$(RANLIB) sike/libsike.a

tests: lib751
	$(CC) $(CFLAGS) -L./sike tests/test_SIKEp751.c tests/test_extras.c -lsike $(LDFLAGS) -o sike/test_KEM $(ARM_SETTING)

# AES
AES_OBJS=objs/aes.o objs/aes_c.o

objs/%.o: tests/aes/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

lib751_for_KATs: $(OBJECTS_751) $(AES_OBJS)
	$(AR) sike/libsike_for_testing.a $^
	$(RANLIB) sike/libsike_for_testing.a

KATS: lib751_for_KATs
	$(CC) $(CFLAGS) -L./sike tests/PQCtestKAT_kem.c tests/rng/rng.c -lsike_for_testing $(LDFLAGS) -o sike/PQCtestKAT_kem $(ARM_SETTING)

check: tests

.PHONY: clean

clean:
	rm -rf *.req objs751 objs sike
