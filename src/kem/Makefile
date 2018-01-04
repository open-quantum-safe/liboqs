HEADERS_KEM=src/kem/kem.h

OBJECTS_KEM=.objs/kem/kem.o

kem_mk_objs_dir:
	@mkdir -p .objs/kem

kem: kem_mk_objs_dir ${OBJECTS_KEM}

.objs/kem/kem.o: src/kem/kem.c
	${CC} -c src/kem/kem.c -o .objs/kem/kem.o ${CFLAGS}

kem_clean:
	rm -rf .objs/kem
