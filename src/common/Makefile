HEADERS_COMMON=src/common/rand.h src/common/common.h

OBJECT_DIRS+=.objs/common
OBJECTS_COMMON=.objs/common/rand.o .objs/common/common.o

.objs/common/rand.o: src/common/rand.c
	$(CC) -c src/common/rand.c -o .objs/common/rand.o $(CFLAGS)

.objs/common/common.o: src/common/common.c
	$(CC) -c src/common/common.c -o .objs/common/common.o $(CFLAGS)
