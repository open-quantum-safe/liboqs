CC=/usr/bin/gcc
LDFLAGS=-lcrypto

SOURCES= cpapke.c fips202.c kem.c ntt.c poly.c PQCgenKAT_kem.c precomp.c reduce.c rng.c verify.c
HEADERS= api.h cpapke.h fips202.h ntt.h params.h poly.h reduce.h rng.h verify.h 

PQCgenKAT_kem: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS)

.PHONY: clean

clean:
	-rm PQCgenKAT_kem
