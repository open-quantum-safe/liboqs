CC = gcc
CFLAGS = -W -Wall -O4
LDFLAGS = -lssl -lcrypto
CPPFLAGS =

OBJECTS = keccak.o kem.o m2e.o permutation.o gf.o poly.o goppa.o decode.o roots.o

.SECONDARY:

.PHONY: force all

all: nistKAT

#
# main targets
#

nistKAT: $(OBJECTS) rng.o PQCgenKAT_kem.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

#
# cleaning rules
#

clean:
	- /bin/rm -f *.o

veryclean: clean
	- /bin/rm nistKAT PQCkemKAT_*.req PQCkemKAT_*.rsp

# DO NOT DELETE

decode.o: goppa.h gf.h poly.h permutation.h
gf.o: gf.h rng.h
goppa.o: goppa.h gf.h poly.h permutation.h
kem.o: kem.h sizes.h gf.h api.h rng.h goppa.h poly.h permutation.h
kem.o: m2e.h
m2e.o: m2e.h sizes.h gf.h api.h
permutation.o: permutation.h rng.h
poly.o: gf.h poly.h
PQCgenKAT_kem.o: rng.h api.h sizes.h gf.h
rng.o: rng.h
roots.o: gf.h poly.h
