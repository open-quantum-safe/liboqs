ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring newhope_512_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring newhope_1024_cca_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_NEWHOPENIST=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring newhope_512_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring newhope_1024_cca_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_NEWHOPENIST=
endif

HEADERS_KEM_NEWHOPENIST=src/kem/newhopenist/kem_newhopenist.h
HEADERS_KEM+=$(HEADERS_KEM_NEWHOPENIST)

OBJECT_DIRS+=.objs/kem/newhopenist
OBJECTS_KEM_NEWHOPENIST=.objs/kem/newhopenist/kem_newhopenist.o
OBJECTS_KEM+=$(OBJECTS_KEM_NEWHOPENIST)

.objs/kem/newhopenist/kem_newhopenist.o: src/kem/newhopenist/kem_newhopenist.c
	$(CC) -c src/kem/newhopenist/kem_newhopenist.c -o .objs/kem/newhopenist/kem_newhopenist.o $(CFLAGS)

ifneq (,$(findstring newhope_512_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_newhope_512_cca_kem_upstream
endif

SRCS_KEM_NEWHOPE_512_CCA_KEM=src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/cpapke.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/fips202.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/kem.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/ntt.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/poly.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/precomp.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/reduce.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/verify.c
OBJS_KEM_NEWHOPE_512_CCA_KEM=$(SRCS_KEM_NEWHOPE_512_CCA_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_NEWHOPE_512_CCA_KEM)

src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/%.o: src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope512cca/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_newhope_512_cca_kem_upstream: $(OBJS_KEM_NEWHOPE_512_CCA_KEM)
	bash scripts/collect_objects.sh kem_newhope_512_cca_kem $(OBJS_KEM_NEWHOPE_512_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_newhope_512_cca_kem src/kem/newhopenist/symbols_global_rename_512.txt
	bash scripts/symbols_local.sh kem_newhope_512_cca_kem src/kem/newhopenist/symbols_local.txt

ifneq (,$(findstring newhope_1024_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_newhope_1024_cca_kem_upstream
endif

SRCS_KEM_NEWHOPE_1024_CCA_KEM=src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/cpapke.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/fips202.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/kem.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/ntt.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/poly.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/precomp.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/reduce.c src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/verify.c
OBJS_KEM_NEWHOPE_1024_CCA_KEM=$(SRCS_KEM_NEWHOPE_1024_CCA_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_NEWHOPE_1024_CCA_KEM)

src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/%.o: src/kem/newhopenist/upstream/Optimized_Implementation/crypto_kem/newhope1024cca/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_newhope_1024_cca_kem_upstream: $(OBJS_KEM_NEWHOPE_1024_CCA_KEM)
	bash scripts/collect_objects.sh kem_newhope_1024_cca_kem $(OBJS_KEM_NEWHOPE_1024_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_newhope_1024_cca_kem src/kem/newhopenist/symbols_global_rename_1024.txt
	bash scripts/symbols_local.sh kem_newhope_1024_cca_kem src/kem/newhopenist/symbols_local.txt
