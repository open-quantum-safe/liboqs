ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring BIG_QUAKE_1, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring BIG_QUAKE_3, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring BIG_QUAKE_5, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_BIGQUAKE=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring BIG_QUAKE_1, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring BIG_QUAKE_3, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring BIG_QUAKE_5, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_BIGQUAKE=
endif

HEADERS_KEM_BIGQUAKE=src/kem/BIG_QUAKE/kem_BIGQUAKE.h
HEADERS_KEM+=$(HEADERS_KEM_BIGQUAKE)

OBJECT_DIRS+=.objs/kem/BIG_QUAKE
OBJECTS_KEM_BIGQUAKE=.objs/kem/BIG_QUAKE/kem_BIGQUAKE.o
OBJECTS_KEM+=$(OBJECTS_KEM_BIGQUAKE)

.objs/kem/BIG_QUAKE/kem_BIGQUAKE.o: headers src/kem/BIG_QUAKE/kem_BIGQUAKE.c
	$(CC) -c src/kem/BIG_QUAKE/kem_BIGQUAKE.c -o .objs/kem/BIG_QUAKE/kem_BIGQUAKE.o $(CFLAGS)

ifneq (,$(findstring BIG_QUAKE_1, $(ENABLE_KEMS)))
UPSTREAMS+=kem_BIG_QUAKE_1_upstream
endif

SRCS_KEM_BIG_QUAKE_1=src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/decode.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/kem.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/permutation.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/poly.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/rng.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/gf.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/goppa.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/roots.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/PQCgenKAT_kem.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/m2e.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/keccak.c
OBJS_KEM_BIG_QUAKE_1=$(SRCS_KEM_BIG_QUAKE_1:.c=.o)

TO_CLEAN+=$(OBJS_KEM_BIG_QUAKE_1)

src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/%.o: src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_1/%.c
	$(CC) -fPIC -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_BIG_QUAKE_1_upstream: $(OBJS_KEM_BIG_QUAKE_1)
	bash scripts/collect_objects.sh kem_BIG_QUAKE_1 $(OBJS_KEM_BIG_QUAKE_1)
	bash scripts/symbols_global_rename.sh kem_BIG_QUAKE_1 src/kem/BIG_QUAKE/symbols_global_rename_1.txt
	bash scripts/symbols_local.sh kem_BIG_QUAKE_1 src/kem/BIG_QUAKE/symbols_local.txt

ifneq (,$(findstring BIG_QUAKE_3, $(ENABLE_KEMS)))
UPSTREAMS+=kem_BIG_QUAKE_3_upstream
endif

SRCS_KEM_BIG_QUAKE_3=src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/decode.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/kem.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/permutation.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/poly.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/rng.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/gf.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/goppa.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/roots.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/m2e.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/keccak.c
OBJS_KEM_BIG_QUAKE_3=$(SRCS_KEM_BIG_QUAKE_3:.c=.o)

TO_CLEAN+=$(OBJS_KEM_BIG_QUAKE_3)

src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/%.o: src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_3/%.c
	$(CC) -fPIC -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_BIG_QUAKE_3_upstream: $(OBJS_KEM_BIG_QUAKE_3)
	bash scripts/collect_objects.sh kem_BIG_QUAKE_3 $(OBJS_KEM_BIG_QUAKE_3)
	bash scripts/symbols_global_rename.sh kem_BIG_QUAKE_3 src/kem/BIG_QUAKE/symbols_global_rename_3.txt
	bash scripts/symbols_local.sh kem_BIG_QUAKE_3 src/kem/BIG_QUAKE/symbols_local.txt

ifneq (,$(findstring BIG_QUAKE_5, $(ENABLE_KEMS)))
UPSTREAMS+=kem_BIG_QUAKE_5_upstream
endif

SRCS_KEM_BIG_QUAKE_5=src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/decode.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/kem.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/permutation.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/poly.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/rng.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/gf.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/goppa.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/roots.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/m2e.c src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/keccak.c
OBJS_KEM_BIG_QUAKE_5=$(SRCS_KEM_BIG_QUAKE_5:.c=.o)

TO_CLEAN+=$(OBJS_KEM_BIG_QUAKE_5)

src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/%.o: src/kem/BIG_QUAKE/upstream/Reference_Implementation/BIG_QUAKE_5/%.c
	$(CC) -fPIC -c -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_BIG_QUAKE_5_upstream: $(OBJS_KEM_BIG_QUAKE_5)
	bash scripts/collect_objects.sh kem_BIG_QUAKE_5 $(OBJS_KEM_BIG_QUAKE_5)
	bash scripts/symbols_global_rename.sh kem_BIG_QUAKE_5 src/kem/BIG_QUAKE/symbols_global_rename_5.txt
	bash scripts/symbols_local.sh kem_BIG_QUAKE_5 src/kem/BIG_QUAKE/symbols_local.txt
