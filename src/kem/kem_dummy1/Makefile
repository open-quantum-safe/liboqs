HEADERS_KEM_DUMMY1=src/kem/kem_dummy1/kem_dummy1.h
HEADERS_KEM+=${HEADERS_KEM_DUMMY1}

OBJECT_DIRS+=.objs/kem/kem_dummy1
OBJECTS_KEM_DUMMY1=.objs/kem/kem_dummy1/kem_dummy1.o
OBJECTS_KEM+=${OBJECTS_KEM_DUMMY1}

ARCHIVES_KEM_DUMMY1=.objs/kem/kem_dummy1/kem_dummy1_upstream.a
ifneq (,$(findstring dummy1, ${ENABLE_KEMS}))
ARCHIVES_KEM+=${ARCHIVES_KEM_DUMMY1}
endif

.objs/kem/kem_dummy1/kem_dummy1.o: src/kem/kem_dummy1/kem_dummy1.c
	${CC} -c src/kem/kem_dummy1/kem_dummy1.c -o .objs/kem/kem_dummy1/kem_dummy1.o ${CFLAGS}

KEM_DUMMY1_UPSTREAM_OBJECTS=src/kem/kem_dummy1/upstream/dummy.o

.objs/kem/kem_dummy1/kem_dummy1_upstream.a:
	cd src/kem/kem_dummy1/upstream && make
	rm -f .objs/kem/kem_dummy1/kem_dummy1_upstream.a
	ar -r -c .objs/kem/kem_dummy1/kem_dummy1_upstream.a ${KEM_DUMMY1_UPSTREAM_OBJECTS}
	ld -r .objs/kem/kem_dummy1/kem_dummy1_upstream.a -o .objs/kem/kem_dummy1/kem_dummy1_upstream.a \
		-alias _crypto_kem_keypair _OQS_KEM_dummy1_keypair \
		-alias _crypto_kem_enc _OQS_KEM_dummy1_encaps \
		-alias _crypto_kem_dec _OQS_KEM_dummy1_decaps \
		-unexported_symbols_list src/kem/kem_dummy1/unexported_symbols_list.txt
	rm -f ${KEM_DUMMY1_UPSTREAM_OBJECTS}
