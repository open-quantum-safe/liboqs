HEADERS_KEM_DUMMY=src/kem/kem_dummy/kem_dummy.h

OBJECT_DIRS+=.objs/kem/kem_dummy
OBJECTS_KEM_DUMMY=.objs/kem/kem_dummy/kem_dummy.o

ARCHIVES_KEM_DUMMY=.objs/kem/kem_dummy/kem_dummy_upstream.a

.objs/kem/kem_dummy/kem_dummy.o: src/kem/kem_dummy/kem_dummy.c
	${CC} -c src/kem/kem_dummy/kem_dummy.c -o .objs/kem/kem_dummy/kem_dummy.o ${CFLAGS}

KEM_DUMMY_UPSTREAM_OBJECTS=src/kem/kem_dummy/upstream/dummy.o

.objs/kem/kem_dummy/kem_dummy_upstream.a:
	cd src/kem/kem_dummy/upstream && make
	rm -f .objs/kem/kem_dummy/kem_dummy_upstream.a
	ar -r -c .objs/kem/kem_dummy/kem_dummy_upstream.a ${KEM_DUMMY_UPSTREAM_OBJECTS}
	ld -r .objs/kem/kem_dummy/kem_dummy_upstream.a -o .objs/kem/kem_dummy/kem_dummy_upstream.a \
		-alias _crypto_kem_keypair _OQS_KEM_dummy_keypair \
		-alias _crypto_kem_enc _OQS_KEM_dummy_encaps \
		-alias _crypto_kem_dec _OQS_KEM_dummy_decaps \
		-unexported_symbols_list src/kem/kem_dummy/unexported_symbols_list.txt
	rm -f ${KEM_DUMMY_UPSTREAM_OBJECTS}
