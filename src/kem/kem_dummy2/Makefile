HEADERS_KEM_DUMMY2=src/kem/kem_dummy2/kem_dummy2.h
HEADERS_KEM+=$(HEADERS_KEM_DUMMY2)

OBJECT_DIRS+=.objs/kem/kem_dummy2
OBJECTS_KEM_DUMMY2=.objs/kem/kem_dummy2/kem_dummy2.o
OBJECTS_KEM+=$(OBJECTS_KEM_DUMMY2)

ARCHIVES_KEM_DUMMY2=.objs/kem/kem_dummy2/kem_dummy2_upstream.a
ifneq (,$(findstring dummy2, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_DUMMY2)
endif

.objs/kem/kem_dummy2/kem_dummy2.o: src/kem/kem_dummy2/kem_dummy2.c
	$(CC) -c src/kem/kem_dummy2/kem_dummy2.c -o .objs/kem/kem_dummy2/kem_dummy2.o $(CFLAGS)

KEM_DUMMY2_UPSTREAM_OBJECTS=src/kem/kem_dummy2/upstream/dummy.o src/kem/kem_dummy2/upstream/extra_file.o

.objs/kem/kem_dummy2/kem_dummy2_upstream.a:
	cd src/kem/kem_dummy2/upstream && $(MAKE)
	rm -f .objs/kem/kem_dummy2/kem_dummy2_upstream.a
	ar -r -c .objs/kem/kem_dummy2/kem_dummy2_upstream.a $(KEM_DUMMY2_UPSTREAM_OBJECTS)
	ld -r .objs/kem/kem_dummy2/kem_dummy2_upstream.a -o .objs/kem/kem_dummy2/kem_dummy2_upstream.a \
		-alias _crypto_kem_keypair _OQS_KEM_dummy2_keypair \
		-alias _crypto_kem_enc _OQS_KEM_dummy2_encaps \
		-alias _crypto_kem_dec _OQS_KEM_dummy2_decaps \
		-unexported_symbols_list src/kem/kem_dummy2/unexported_symbols_list.txt
	rm -f $(KEM_DUMMY2_UPSTREAM_OBJECTS)
