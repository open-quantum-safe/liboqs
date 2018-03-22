ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring kyber512, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring kyber768, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring kyber1024, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_KYBER=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring kyber512, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring kyber768, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring kyber1024, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_KYBER=
endif

HEADERS_KEM_KYBER=src/kem/crystals-kyber/kem_kyber.h
HEADERS_KEM+=$(HEADERS_KEM_KYBER)

OBJECT_DIRS+=.objs/kem/crystals-kyber
OBJECTS_KEM_KYBER=.objs/kem/crystals-kyber/kem_kyber.o
OBJECTS_KEM+=$(OBJECTS_KEM_KYBER)

.objs/kem/crystals-kyber/kem_kyber.o: src/kem/crystals-kyber/kem_kyber.c
	$(CC) -c src/kem/crystals-kyber/kem_kyber.c -o .objs/kem/crystals-kyber/kem_kyber.o $(CFLAGS)

ARCHIVES_KEM_KYBER512_CCA_KEM=.objs/kem/crystals-kyber/kem_kyber512_upstream.a
ifneq (,$(findstring kyber512, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_KYBER512_CCA_KEM)
endif

SRCS_KEM_KYBER512_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/rng.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/verify.c
OBJS_KEM_KYBER512_CCA_KEM=$(SRCS_KEM_KYBER512_CCA_KEM:.c=.o)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/%.c
	$(CC) -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

$(ARCHIVES_KEM_KYBER512_CCA_KEM): $(OBJS_KEM_KYBER512_CCA_KEM)
	$(RM) $@
	ar -r -c $@ $(OBJS_KEM_KYBER512_CCA_KEM)
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair OQS_KEM_kyber512_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc OQS_KEM_kyber512_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec OQS_KEM_kyber512_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/crystals-kyber/unexported_symbols_list.txt
	$(RM) $(OBJS_KEM_KYBER512_CCA_KEM)

ARCHIVES_KEM_KYBER768_CCA_KEM=.objs/kem/crystals-kyber/kem_kyber768_upstream.a
ifneq (,$(findstring kyber768, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_KYBER768_CCA_KEM)
endif

SRCS_KEM_KYBER768_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/rng.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/verify.c
OBJS_KEM_KYBER768_CCA_KEM=$(SRCS_KEM_KYBER768_CCA_KEM:.c=.o)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/%.c
	$(CC) -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

$(ARCHIVES_KEM_KYBER768_CCA_KEM): $(OBJS_KEM_KYBER768_CCA_KEM)
	$(RM) $@
	ar -r -c $@ $(OBJS_KEM_KYBER768_CCA_KEM)
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair OQS_KEM_kyber768_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc OQS_KEM_kyber768_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec OQS_KEM_kyber768_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/crystals-kyber/unexported_symbols_list.txt
	$(RM) $(OBJS_KEM_KYBER768_CCA_KEM)

ARCHIVES_KEM_KYBER1024_CCA_KEM=.objs/kem/crystals-kyber/kem_kyber1024_upstream.a
ifneq (,$(findstring kyber1024, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_KYBER1024_CCA_KEM)
endif

SRCS_KEM_KYBER1024_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/rng.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/verify.c
OBJS_KEM_KYBER1024_CCA_KEM=$(SRCS_KEM_KYBER1024_CCA_KEM:.c=.o)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/%.c
	$(CC) -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

$(ARCHIVES_KEM_KYBER1024_CCA_KEM): $(OBJS_KEM_KYBER1024_CCA_KEM)
	$(RM) $@
	ar -r -c $@ $(OBJS_KEM_KYBER1024_CCA_KEM)
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair OQS_KEM_kyber1024_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc OQS_KEM_kyber1024_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec OQS_KEM_kyber1024_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/crystals-kyber/unexported_symbols_list.txt
	$(RM) $(OBJS_KEM_KYBER1024_CCA_KEM)
