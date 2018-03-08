ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring BIG_QUAKE_1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_BIGQUAKE=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring BIG_QUAKE_1, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_BIGQUAKE=
endif

HEADERS_KEM_BIGQUAKE=src/kem/BIG_QUAKE/kem_BIGQUAKE.h
HEADERS_KEM+=$(HEADERS_KEM_BIGQUAKE)

OBJECT_DIRS+=.objs/kem/BIG_QUAKE
OBJECTS_KEM_BIGQUAKE=.objs/kem/BIG_QUAKE/kem_BIGQUAKE.o
OBJECTS_KEM+=$(OBJECTS_KEM_BIGQUAKE)

.objs/kem/BIG_QUAKE/kem_BIGQUAKE.o: src/kem/BIG_QUAKE/kem_BIGQUAKE.c
	$(CC) -c src/kem/BIG_QUAKE/kem_BIGQUAKE.c -o .objs/kem/BIG_QUAKE/kem_BIGQUAKE.o $(CFLAGS)

ARCHIVES_KEM_BIG_QUAKE_1=.objs/kem/BIG_QUAKE/kem_BIG_QUAKE_1_upstream.a
ifneq (,$(findstring BIG_QUAKE_1, $(ENABLE_KEMS)))
ARCHIVES_KEM+=$(ARCHIVES_KEM_BIG_QUAKE_1)
endif

SRCS_KEM_BIG_QUAKE_1=src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/decode.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/kem.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/permutation.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/poly.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/rng.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/gf.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/goppa.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/roots.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/PQCgenKAT_kem.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/m2e.c src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/keccak.c
OBJS_KEM_BIG_QUAKE_1=$(SRCS_KEM_BIG_QUAKE_1:.c=.o)

src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/%.o: src/kem/BIG_QUAKE/upstream/Optimized_Implementation/BIG_QUAKE_1/%.c
	$(CC) -c -O3 -std=c11 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

$(ARCHIVES_KEM_BIG_QUAKE_1): $(OBJS_KEM_BIG_QUAKE_1)
	$(RM) $@
	ar -r -c $@ $(OBJS_KEM_BIG_QUAKE_1)
	bash scripts/symbol_alias.sh $@ crypto_kem_keypair OQS_KEM_BIG_QUAKE_1_keypair
	bash scripts/symbol_alias.sh $@ crypto_kem_enc OQS_KEM_BIG_QUAKE_1_encaps
	bash scripts/symbol_alias.sh $@ crypto_kem_dec OQS_KEM_BIG_QUAKE_1_decaps
	bash scripts/symbol_unexport.sh $@ src/kem/BIG_QUAKE/unexported_symbols_list.txt
	$(RM) $(OBJS_KEM_BIG_QUAKE_1)

