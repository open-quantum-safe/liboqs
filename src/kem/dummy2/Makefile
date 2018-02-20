ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring dummy2, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY2=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring dummy2, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_DUMMY2=
endif

HEADERS_KEM_DUMMY2=src/kem/dummy2/kem_dummy2.h
HEADERS_KEM+=$(HEADERS_KEM_DUMMY2)

OBJECT_DIRS+=.objs/kem/dummy2
OBJECTS_KEM_DUMMY2=.objs/kem/dummy2/kem_dummy2.o
OBJECTS_KEM+=$(OBJECTS_KEM_DUMMY2)

.objs/kem/dummy2/kem_dummy2.o: src/kem/dummy2/kem_dummy2.c
	$(CC) -c src/kem/dummy2/kem_dummy2.c -o .objs/kem/dummy2/kem_dummy2.o $(CFLAGS)

ARCHIVES_KEM_DUMMY2=.objs/kem/dummy2/kem_dummy2_upstream.a
ifneq (,$(findstring dummy2, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_DUMMY2)
endif

KEM_DUMMY2_UPSTREAM_OBJECTS=src/kem/dummy2/upstream/dummy.o src/kem/dummy2/upstream/extra_file.o

.objs/kem/dummy2/kem_dummy2_upstream.a:
	cd src/kem/dummy2/upstream && $(MAKE) $(MAKE_FLAGS_KEM_DUMMY2)
	$(RM) $@
	ar -r -c $@ $(KEM_DUMMY2_UPSTREAM_OBJECTS)
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair OQS_KEM_dummy2_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc OQS_KEM_dummy2_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec OQS_KEM_dummy2_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/dummy2/unexported_symbols_list.txt
	$(RM) $(KEM_DUMMY2_UPSTREAM_OBJECTS)
