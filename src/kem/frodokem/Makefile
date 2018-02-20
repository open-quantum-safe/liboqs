ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring frodokem_640_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_640_cshake, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_cshake, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_FRODOKEM=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring frodokem_640_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_640_cshake, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_cshake, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_FRODOKEM=ARCH=x86
endif

HEADERS_KEM_FRODOKEM=src/kem/frodokem/kem_frodokem.h
HEADERS_KEM+=$(HEADERS_KEM_FRODOKEM)

OBJECT_DIRS+=.objs/kem/frodokem
OBJECTS_KEM_FRODOKEM=.objs/kem/frodokem/kem_frodokem.o
OBJECTS_KEM+=$(OBJECTS_KEM_FRODOKEM)

.objs/kem/frodokem/kem_frodokem.o: src/kem/frodokem/kem_frodokem.c
	$(CC) -c src/kem/frodokem/kem_frodokem.c -o .objs/kem/frodokem/kem_frodokem.o $(CFLAGS)

ARCHIVES_KEM_FRODOKEM_640_AES=.objs/kem/frodokem/kem_frodokem_640_aes_upstream.a
ifneq (,$(findstring frodokem_640_aes, $(ENABLE_KEMS)))
ARCHIVES_KEM_FRODOKEM+=$(ARCHIVES_KEM_FRODOKEM_640_AES)
endif

ARCHIVES_KEM_FRODOKEM_976_AES=.objs/kem/frodokem/kem_frodokem_976_aes_upstream.a
ifneq (,$(findstring frodokem_976_aes, $(ENABLE_KEMS)))
ARCHIVES_KEM_FRODOKEM+=$(ARCHIVES_KEM_FRODOKEM_976_AES)
endif

ARCHIVES_KEM_FRODOKEM_640_CSHAKE=.objs/kem/frodokem/kem_frodokem_640_cshake_upstream.a
ifneq (,$(findstring frodokem_640_cshake, $(ENABLE_KEMS)))
ARCHIVES_KEM_FRODOKEM+=$(ARCHIVES_KEM_FRODOKEM_640_CSHAKE)
endif

ARCHIVES_KEM_FRODOKEM_976_CSHAKE=.objs/kem/frodokem/kem_frodokem_976_cshake_upstream.a
ifneq (,$(findstring frodokem_976_cshake, $(ENABLE_KEMS)))
ARCHIVES_KEM_FRODOKEM+=$(ARCHIVES_KEM_FRODOKEM_976_CSHAKE)
endif

ARCHIVES_KEM+=$(ARCHIVES_KEM_FRODOKEM)

.objs/kem/frodokem/kem_frodokem_640_aes_upstream.a:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=AES128 USE_OPENSSL=FALSE
	$(RM) $@
	cp src/kem/frodokem/upstream/frodo640/libfrodo.a $@
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair_Frodo640 OQS_KEM_frodokem_640_aes_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc_Frodo640 OQS_KEM_frodokem_640_aes_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec_Frodo640 OQS_KEM_frodokem_640_aes_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/frodokem/unexported_symbols_list.txt
	cd src/kem/frodokem/upstream && $(MAKE) clean

.objs/kem/frodokem/kem_frodokem_976_aes_upstream.a:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=AES128 USE_OPENSSL=FALSE
	$(RM) $@
	cp src/kem/frodokem/upstream/frodo976/libfrodo.a $@
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair_Frodo976 OQS_KEM_frodokem_976_aes_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc_Frodo976 OQS_KEM_frodokem_976_aes_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec_Frodo976 OQS_KEM_frodokem_976_aes_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/frodokem/unexported_symbols_list.txt
	cd src/kem/frodokem/upstream && $(MAKE) clean

.objs/kem/frodokem/kem_frodokem_640_cshake_upstream.a:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=CSHAKE128 USE_OPENSSL=FALSE
	$(RM) $@
	cp src/kem/frodokem/upstream/frodo640/libfrodo.a $@
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair_Frodo640 OQS_KEM_frodokem_640_cshake_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc_Frodo640 OQS_KEM_frodokem_640_cshake_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec_Frodo640 OQS_KEM_frodokem_640_cshake_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/frodokem/unexported_symbols_list.txt
	cd src/kem/frodokem/upstream && $(MAKE) clean

.objs/kem/frodokem/kem_frodokem_976_cshake_upstream.a:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=CSHAKE128 USE_OPENSSL=FALSE
	$(RM) $@
	cp src/kem/frodokem/upstream/frodo976/libfrodo.a $@
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair_Frodo976 OQS_KEM_frodokem_976_cshake_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc_Frodo976 OQS_KEM_frodokem_976_cshake_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec_Frodo976 OQS_KEM_frodokem_976_cshake_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/frodokem/unexported_symbols_list.txt
	cd src/kem/frodokem/upstream && $(MAKE) clean
