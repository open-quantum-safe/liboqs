HEADERS_KEM=src/kem/kem.h

OBJECT_DIRS+=.objs/kem
OBJECTS_KEM=.objs/kem/kem.o
ARCHIVES_KEM=

.objs/kem/kem.o: src/kem/kem.c
	${CC} -c src/kem/kem.c -o .objs/kem/kem.o ${CFLAGS}

test_kem: src/kem/test_kem.c liboqs.a
	${CC} src/kem/test_kem.c liboqs.a -o test_kem ${CFLAGS} ${LDFLAGS}

include src/kem/kem_dummy1/Makefile
HEADERS_KEM+=${HEADERS_KEM_DUMMY1}
OBJECTS_KEM+=${OBJECTS_KEM_DUMMY1}
ifneq (,$(findstring dummy1, ${ENABLE_KEMS}))
ARCHIVES_KEM+=${ARCHIVES_KEM_DUMMY1}
endif

include src/kem/kem_dummy2/Makefile
HEADERS_KEM+=${HEADERS_KEM_DUMMY2}
OBJECTS_KEM+=${OBJECTS_KEM_DUMMY2}
ifneq (,$(findstring dummy2, ${ENABLE_KEMS}))
ARCHIVES_KEM+=${ARCHIVES_KEM_DUMMY2}
endif
