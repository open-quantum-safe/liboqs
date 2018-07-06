CC = /usr/bin/gcc
CFLAGS = -std=gnu11 -O3 -march=native -fomit-frame-pointer #-Wall -Wextra 
LDFLAGS=-lm -L/usr/lib/ -lssl -lcrypto 

DFLAG=
ifeq "$(DEBUG)" "TRUE"
    DFLAG=-DDEBUG
endif

SOURCE = sign.c sample.c poly.c consts.c sha3/fips202.c
HEADER = api.h params.h sample.h sha3/fips202.h
SOURCE_TEST = random/random.c tests/cpucycles.c tests/test_qtesla.c
HEADER_TEST = random/random.h tests/cpucycles.h
SOURCE_KATS_GEN  = tests/rng.c tests/PQCgenKAT_sign.c
SOURCE_KATS_TEST = tests/rng.c tests/PQCtestKAT_sign.c
HEADER_KATS = tests/rng.h

all: test_qtesla PQCgenKAT_sign PQCtestKAT_sign

test_qtesla: $(SOURCE) $(SOURCE_TEST) $(HEADER)
	$(CC) $(CFLAGS) $(SOURCE) $(SOURCE_TEST) $(DFLAG) $(LDFLAGS) -o $@

PQCgenKAT_sign: $(SOURCE) $(SOURCE_KATS_GEN) $(HEADER) $(HEADER_KATS)
	$(CC) $(CFLAGS) $(SOURCE) $(SOURCE_KATS_GEN) $(LDFLAGS) -o $@

PQCtestKAT_sign: $(SOURCE) $(SOURCE_KATS_TEST) $(HEADER) $(HEADER_KATS)
	$(CC) $(CFLAGS) $(SOURCE) $(SOURCE_KATS_TEST) $(LDFLAGS) -o $@

.PHONY: clean

clean:
	rm -rf test_qtesla PQCgenKAT_sign PQCtestKAT_sign PQCsignKAT_qTesla*
