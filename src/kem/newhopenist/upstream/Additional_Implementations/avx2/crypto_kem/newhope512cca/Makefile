CC=/usr/bin/gcc
CFLAGS=-no-pie -O3 -mavx2
LDFLAGS=-lcrypto

SOURCES= cpapke.c fips202.c kem.c ntt.c poly.c PQCgenKAT_kem.c precomp.c reduce.c rng.c verify.c consts.c fips202x4.c ntt_double.s \
         keccak4x/KeccakP-1600-times4-SIMD256.c
HEADERS= api.h cpapke.h fips202.h ntt.h params.h poly.h reduce.h rng.h verify.h fips202x4.h \
         keccak4x/align.h keccak4x/brg_endian.h keccak4x/KeccakP-1600-times4-SnP.h keccak4x/KeccakP-1600-unrolling.macros keccak4x/SIMD256-config.h

PQCgenKAT_kem: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

.PHONY: clean

clean:
	-rm PQCgenKAT_kem

