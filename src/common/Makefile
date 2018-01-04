HEADERS_COMMON=src/common/rand.h

OBJECTS_COMMON=.objs/common/rand.o

common_mk_objs_dir:
	@mkdir -p .objs/common

common: common_mk_objs_dir ${OBJECTS_COMMON}

.objs/common/rand.o: src/common/rand.c
	${CC} -c src/common/rand.c -o .objs/common/rand.o ${CFLAGS}

common_clean:
	rm -rf .objs/common
