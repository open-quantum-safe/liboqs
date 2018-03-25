ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring dummy2, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY2=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring dummy2, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY2=
endif

HEADERS_KEM_DUMMY2=src/kem/dummy2/kem_dummy2.h
HEADERS_KEM+=$(HEADERS_KEM_DUMMY2)

OBJECT_DIRS+=.objs/kem_dummy2
OBJECTS_KEM_DUMMY2=.objs/kem_dummy2/kem_dummy2.o
OBJECTS_KEM+=$(OBJECTS_KEM_DUMMY2)

.objs/kem_dummy2/kem_dummy2.o: src/kem/dummy2/kem_dummy2.c
	$(CC) -c src/kem/dummy2/kem_dummy2.c -o .objs/kem_dummy2/kem_dummy2.o $(CFLAGS)

ifneq (,$(findstring dummy2, $(ENABLE_KEMS)))
UPSTREAMS+=kem_dummy2_upstream
endif

KEM_DUMMY2_UPSTREAM_OBJECTS=src/kem/dummy2/upstream/dummy.o src/kem/dummy2/upstream/extra_file.o
kem_dummy2_upstream:
	cd src/kem/dummy2/upstream && $(MAKE) $(MAKE_FLAGS_KEM_DUMMY2)
	bash scripts/collect_objects.sh kem_dummy2 $(KEM_DUMMY2_UPSTREAM_OBJECTS)
	bash scripts/symbols_global_rename.sh kem_dummy2 src/kem/dummy2/symbols_global_rename.txt
	bash scripts/symbols_local.sh kem_dummy2 src/kem/dummy2/symbols_local.txt
