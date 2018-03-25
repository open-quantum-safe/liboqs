ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring dummy1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY1=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring dummy1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY1=
endif

HEADERS_KEM_DUMMY1=src/kem/dummy1/kem_dummy1.h
HEADERS_KEM+=$(HEADERS_KEM_DUMMY1)

OBJECT_DIRS+=.objs/kem_dummy1
OBJECTS_KEM_DUMMY1=.objs/kem_dummy1/kem_dummy1.o
OBJECTS_KEM+=$(OBJECTS_KEM_DUMMY1)

.objs/kem_dummy1/kem_dummy1.o: src/kem/dummy1/kem_dummy1.c
	$(CC) -c src/kem/dummy1/kem_dummy1.c -o .objs/kem_dummy1/kem_dummy1.o $(CFLAGS)

ifneq (,$(findstring dummy1, $(ENABLE_KEMS)))
UPSTREAMS+=kem_dummy1_upstream
endif

KEM_DUMMY1_UPSTREAM_OBJECTS=src/kem/dummy1/upstream/dummy.o
kem_dummy1_upstream:
	cd src/kem/dummy1/upstream && $(MAKE) $(MAKE_FLAGS_KEM_DUMMY1)
	bash scripts/collect_objects.sh kem_dummy1 $(KEM_DUMMY1_UPSTREAM_OBJECTS)
	bash scripts/symbols_global_rename.sh kem_dummy1 src/kem/dummy1/symbols_global_rename.txt
	bash scripts/symbols_local.sh kem_dummy1 src/kem/dummy1/symbols_local.txt
