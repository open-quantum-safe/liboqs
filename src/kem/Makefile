HEADERS_KEM=src/kem/kem.h

OBJECT_DIRS+=.objs/kem
OBJECTS_KEM=.objs/kem/kem.o

.objs/kem/kem.o: headers src/kem/kem.c
	$(CC) -c src/kem/kem.c -o .objs/kem/kem.o $(CFLAGS)

include src/kem/frodokem/Makefile
include src/kem/newhopenist/Makefile
include src/kem/crystals-kyber/Makefile
include src/kem/bike/Makefile
include src/kem/sike/Makefile
include src/kem/BIG_QUAKE/Makefile
include src/kem/saber/Makefile
include src/kem/lima/Makefile
include src/kem/ledakem/Makefile
include src/kem/threebears/Makefile
include src/kem/titanium/Makefile
# EDIT-WHEN-ADDING-KEM
