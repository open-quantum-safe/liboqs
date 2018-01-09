ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring dummy1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY1=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring dummy1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY1=
endif

HEADERS_KEM_DUMMY1=src/kem/dummy1/kem_dummy1.h
HEADERS_KEM+=$(HEADERS_KEM_DUMMY1)

OBJECT_DIRS+=.objs/kem/dummy1
OBJECTS_KEM_DUMMY1=.objs/kem/dummy1/kem_dummy1.o
OBJECTS_KEM+=$(OBJECTS_KEM_DUMMY1)

.objs/kem/dummy1/kem_dummy1.o: src/kem/dummy1/kem_dummy1.c
	$(CC) -c src/kem/dummy1/kem_dummy1.c -o .objs/kem/dummy1/kem_dummy1.o $(CFLAGS)

ARCHIVES_KEM_DUMMY1=.objs/kem/dummy1/kem_dummy1_upstream.a
ifneq (,$(findstring dummy1, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_DUMMY1)
endif

KEM_DUMMY1_UPSTREAM_OBJECTS=src/kem/dummy1/upstream/dummy.o

.objs/kem/dummy1/kem_dummy1_upstream.a:
	cd src/kem/dummy1/upstream && $(MAKE) $(MAKE_FLAGS_KEM_DUMMY1)
	$(RM) $@
	ar -r -c $@ $(KEM_DUMMY1_UPSTREAM_OBJECTS)
	bash scripts/symbol_alias.sh $@ _crypto_kem_keypair _OQS_KEM_dummy1_keypair
	bash scripts/symbol_alias.sh $@ _crypto_kem_enc _OQS_KEM_dummy1_encaps
	bash scripts/symbol_alias.sh $@ _crypto_kem_dec _OQS_KEM_dummy1_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/dummy1/unexported_symbols_list.txt
	$(RM) $(KEM_DUMMY1_UPSTREAM_OBJECTS)
