LEDA_KEM=$(findstring ledakem_C1_N02, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C1_N02)
SL=1
N0=2
else
LEDA_KEM=$(findstring ledakem_C1_N03, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C1_N03)
SL=1
N0=3
else
LEDA_KEM=$(findstring ledakem_C1_N04, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C1_N04)
SL=1
N0=4
else
LEDA_KEM=$(findstring ledakem_C2_N02, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C2_N02)
SL=2
N0=2
else
LEDA_KEM=$(findstring ledakem_C2_N03, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C2_N03)
SL=2
N0=3
else
LEDA_KEM=$(findstring ledakem_C2_N04, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C2_N04)
SL=2
N0=4
else
LEDA_KEM=$(findstring ledakem_C3_N02, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C3_N02)
SL=3
N0=2
else
LEDA_KEM=$(findstring ledakem_C3_N03, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C3_N03)
SL=3
N0=3
else
LEDA_KEM=$(findstring ledakem_C3_N04, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C3_N04)
SL=3
N0=4
else
LEDA_KEM=$(findstring ledakem_C4_N02, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C4_N02)
SL=4
N0=2
else
LEDA_KEM=$(findstring ledakem_C4_N03, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C4_N03)
SL=4
N0=3
else
LEDA_KEM=$(findstring ledakem_C4_N04, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C4_N04)
SL=4
N0=4
else
LEDA_KEM=$(findstring ledakem_C5_N02, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C5_N02)
SL=5
N0=2
else
LEDA_KEM=$(findstring ledakem_C5_N03, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C5_N03)
SL=5
N0=3
else
LEDA_KEM=$(findstring ledakem_C5_N04, ${KEMS_TO_ENABLE})
ifeq (${LEDA_KEM}, ledakem_C5_N04)
SL=5
N0=4
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

ifeq (x64,$(ARCH))
ENABLE_KEMS+=${LEDA_KEM}
MAKE_FLAGS_KEM_LEDAKEM=
AKE_FLAGS_KEM_LEDAKEM=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=${LEDA_KEM}
MAKE_FLAGS_KEM_LEDAKEM=
AKE_FLAGS_KEM_LEDAKEM=ARCH=x86
endif

HEADERS_KEM_LEDAKEM=src/kem/ledakem/kem_LEDAkem.h
HEADERS_KEM+=$(HEADERS_KEM_LEDAKEM)

OBJECT_DIRS+=.objs/kem/ledakem
OBJECTS_KEM_LEDAKEM=.objs/kem/ledakem/kem_LEDAkem.o
OBJECTS_KEM+=$(OBJECTS_KEM_LEDAKEM)

.objs/kem/ledakem/kem_LEDAkem.o: src/kem/ledakem/kem_LEDAkem.c
	$(CC) -c src/kem/ledakem/kem_LEDAkem.c -o .objs/kem/ledakem/kem_LEDAkem.o $(CFLAGS)

ifneq (,$(findstring ${LEDA_KEM}, $(ENABLE_KEMS)))
UPSTREAMS+=kem_LEDA_KEM_upstream
endif

SRCS_KEM_LEDA_KEM=src/kem/ledakem/upstream/Reference_Implementation/KEM/library/kem.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/rng.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/sha3.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/niederreiter_encrypt.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/H_Q_matrices_generation.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/aes256.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/niederreiter_keygen.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/gf2x_arith.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/bf_decoding.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/niederreiter_decrypt.c src/kem/ledakem/upstream/Reference_Implementation/KEM/library/gf2x_arith_mod_xPplusOne.c

OBJS_KEM_LEDA_KEM=$(SRCS_KEM_LEDA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_LEDA_KEM)

src/kem/ledakem/upstream/Reference_Implementation/KEM/library/%.o: src/kem/ledakem/upstream/Reference_Implementation/KEM/library/%.c
	$(CC) -fPIC -c -O3 -std=c99  -DCATEGORY=$(SL) -DN0=$(N0) -DCPU_WORD_BITS=64 -o $@ $< -I$(OPENSSL_INCLUDE_DIR) -Isrc/kem/ledakem/upstream/Reference_Implementation/KEM/include

kem_LEDA_KEM_upstream: $(OBJS_KEM_LEDA_KEM)
	bash scripts/collect_objects.sh kem_LEDA_KEM $(OBJS_KEM_LEDA_KEM)
	bash scripts/symbols_global_rename.sh kem_LEDA_KEM src/kem/ledakem/symbols_global_rename.txt
	bash scripts/symbols_local.sh kem_LEDA_KEM src/kem/ledakem/symbols_local.txt

