HEADERS_KEM=src/kem/kem.h

OBJECT_DIRS+=.objs/kem
OBJECTS_KEM=.objs/kem/kem.o

.objs/kem/kem.o: src/kem/kem.c
	$(CC) -c src/kem/kem.c -o .objs/kem/kem.o $(CFLAGS)

test_kem: src/kem/test_kem.c liboqs
	$(CC) src/kem/test_kem.c liboqs.a -o test_kem $(CFLAGS) $(LDFLAGS)

test_kem_shared: src/kem/test_kem.c liboqs
	$(CC) src/kem/test_kem.c -o test_kem_shared $(CFLAGS) $(LDFLAGS) -L. -loqs -lcrypto -lm

kat_kem: src/kem/kat_kem.c liboqs
	$(CC) src/kem/kat_kem.c liboqs.a -o kat_kem $(CFLAGS) $(LDFLAGS)

speed_kem: src/kem/speed_kem.c liboqs
	$(CC) src/kem/speed_kem.c liboqs.a -o speed_kem $(CFLAGS) $(LDFLAGS)

example_kem: src/kem/example_kem.c liboqs
	$(CC) src/kem/example_kem.c liboqs.a -o example_kem $(CFLAGS) $(LDFLAGS)

include src/kem/frodokem/Makefile
include src/kem/newhopenist/Makefile
include src/kem/crystals-kyber/Makefile
include src/kem/bike/Makefile
include src/kem/sike/Makefile
include src/kem/BIG_QUAKE/Makefile
# EDIT-WHEN-ADDING-KEM
