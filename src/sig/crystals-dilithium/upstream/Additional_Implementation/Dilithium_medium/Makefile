CC = /usr/bin/cc
CFLAGS = -Wall -Wextra -march=native -mtune=native -O3 -fomit-frame-pointer
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -fPIC
SOURCES = sign.c polyvec.c packing.c poly.c reduce.c ntt.s invntt.s \
  pointwise_mul.s consts.c rounding.c fips202.c fips202x4.c \
  keccak4x/KeccakP-1600-times4-SIMD256.o
HEADERS = api.h params.h sign.h polyvec.h packing.h poly.h reduce.h ntt.h \
  rounding.h fips202.h fips202x4.h

all: PQCgenKAT_sign test/test_vectors test/test_dilithium

keccak4x/KeccakP-1600-times4-SIMD256.o: keccak4x/KeccakP-1600-times4-SIMD256.c \
  keccak4x/align.h keccak4x/brg_endian.h keccak4x/KeccakP-1600-times4-SnP.h \
  keccak4x/KeccakP-1600-unrolling.macros keccak4x/SIMD256-config.h
	$(CC) $(CFLAGS) -c $< -o $@

PQCgenKAT_sign: $(SOURCES) rng.c PQCgenKAT_sign.c $(HEADERS) rng.h
	$(CC) $(NISTFLAGS) $(SOURCES) rng.c PQCgenKAT_sign.c -o $@ -lcrypto -ldl

test/test_vectors: $(SOURCES) rng.c test/test_vectors.c $(HEADERS) rng.h
	$(CC) $(NISTFLAGS) $(SOURCES) rng.c test/test_vectors.c -o $@ -lcrypto -ldl

test/test_dilithium: $(SOURCES) randombytes.c test/test_dilithium.c \
  test/speed.c test/cpucycles.c $(HEADERS) randombytes.h test/speed.h \
  test/cpucycles.h
	$(CC) $(CFLAGS) $(SOURCES) randombytes.c test/test_dilithium.c \
	  test/speed.c test/cpucycles.c -o $@

test/test_mul: ntt.s invntt.s pointwise_mul.s consts.c randombytes.c \
  test/test_mul.c test/speed.c test/cpucycles.c params.h ntt.h randombytes.h \
  test/speed.h test/cpucycles.h
	$(CC) $(CFLAGS) ntt.s invntt.s pointwise_mul.s consts.c randombytes.c \
          test/test_mul.c test/speed.c test/cpucycles.c -o $@

.PHONY: clean

clean:
	rm -f *~ test/*~
	rm -f keccak4x/KeccakP-1600-times4-SIMD256.o
	rm -f PQCgenKAT_sign
	rm -f test/test_vectors
	rm -f test/test_dilithium
	rm -f test/test_mul
