HEADERS_SIG=src/sig/sig.h

OBJECT_DIRS+=.objs/sig
OBJECTS_SIG=.objs/sig/sig.o

.objs/sig/sig.o: headers src/sig/sig.c
	$(CC) -c src/sig/sig.c -o .objs/sig/sig.o $(CFLAGS)

include src/sig/picnic/Makefile
include src/sig/qtesla/Makefile
include src/sig/crystals-dilithium/Makefile
# EDIT-WHEN-ADDING-SIG
