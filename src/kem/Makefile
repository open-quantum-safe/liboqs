HEADERS_KEM=src/kem/kem.h

OBJECT_DIRS+=.objs/kem
OBJECTS_KEM=.objs/kem/kem.o
ARCHIVES_KEM=

.objs/kem/kem.o: src/kem/kem.c
	$(CC) -c src/kem/kem.c -o .objs/kem/kem.o $(CFLAGS)

test_kem: src/kem/test_kem.c liboqs
	$(CC) src/kem/test_kem.c liboqs.a -o test_kem $(CFLAGS) $(LDFLAGS)

speed_kem: src/kem/speed_kem.c liboqs
	$(CC) src/kem/speed_kem.c liboqs.a -o speed_kem $(CFLAGS) $(LDFLAGS)

example_kem: src/kem/example_kem.c liboqs
	$(CC) src/kem/example_kem.c liboqs.a -o example_kem $(CFLAGS) $(LDFLAGS)

include src/kem/dummy1/Makefile
include src/kem/dummy2/Makefile
include src/kem/frodokem/Makefile
include src/kem/newhopenist/Makefile
# EDIT-WHEN-ADDING-KEM
