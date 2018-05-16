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

.objs/kem/crystals-kyber/kem_kyber.o: headers src/kem/crystals-kyber/kem_kyber.c
	$(CC) -c src/kem/crystals-kyber/kem_kyber.c -o .objs/kem/crystals-kyber/kem_kyber.o $(CFLAGS)

ifneq (,$(findstring kyber512, $(ENABLE_KEMS)))
UPSTREAMS+=kem_kyber512_upstream
endif

SRCS_KEM_KYBER512_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/verify.c
OBJS_KEM_KYBER512_CCA_KEM=$(SRCS_KEM_KYBER512_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_KYBER512_CCA_KEM)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber512/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_kyber512_upstream: $(OBJS_KEM_KYBER512_CCA_KEM)
	bash scripts/collect_objects.sh kem_kyber512 $(OBJS_KEM_KYBER512_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_kyber512 src/kem/crystals-kyber/symbols_global_rename_512.txt
	bash scripts/symbols_local.sh kem_kyber512 src/kem/crystals-kyber/symbols_local.txt

ifneq (,$(findstring kyber768, $(ENABLE_KEMS)))
UPSTREAMS+=kem_kyber768_upstream
endif

SRCS_KEM_KYBER768_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/verify.c
OBJS_KEM_KYBER768_CCA_KEM=$(SRCS_KEM_KYBER768_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_KYBER768_CCA_KEM)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber768/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_kyber768_upstream: $(OBJS_KEM_KYBER768_CCA_KEM)
	bash scripts/collect_objects.sh kem_kyber768 $(OBJS_KEM_KYBER768_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_kyber768 src/kem/crystals-kyber/symbols_global_rename_768.txt
	bash scripts/symbols_local.sh kem_kyber768 src/kem/crystals-kyber/symbols_local.txt

ifneq (,$(findstring kyber1024, $(ENABLE_KEMS)))
UPSTREAMS+=kem_kyber1024_usptream
endif

SRCS_KEM_KYBER1024_CCA_KEM=src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/fips202.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/kem.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/poly.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/precomp.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/cbd.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/indcpa.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/ntt.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/polyvec.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/reduce.c \
src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/verify.c
OBJS_KEM_KYBER1024_CCA_KEM=$(SRCS_KEM_KYBER1024_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_KYBER1024_CCA_KEM)

src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/%.o: src/kem/crystals-kyber/upstream/Optimized_Implementation/crypto_kem/kyber1024/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_kyber1024_usptream: $(OBJS_KEM_KYBER1024_CCA_KEM)
	bash scripts/collect_objects.sh kem_kyber1024 $(OBJS_KEM_KYBER1024_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_kyber1024 src/kem/crystals-kyber/symbols_global_rename_1024.txt
	bash scripts/symbols_local.sh kem_kyber1024 src/kem/crystals-kyber/symbols_local.txt
