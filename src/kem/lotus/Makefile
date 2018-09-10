ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring LOTUS192KEM, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring LOTUS256KEM, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring LOTUS128KEM, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_LOTUS=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring LOTUS192KEM, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring LOTUS256KEM, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring LOTUS128KEM, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_LOTUS=
endif

HEADERS_KEM_LOTUS=src/kem/lotus/kem_lotus.h
HEADERS_KEM+=$(HEADERS_KEM_LOTUS)

OBJECT_DIRS+=.objs/kem/lotus
OBJECTS_KEM_LOTUS=.objs/kem/lotus/kem_lotus.o
OBJECTS_KEM+=$(OBJECTS_KEM_LOTUS)

# build liboqs interface to upstream component
.objs/kem/lotus/kem_lotus.o: headers src/kem/lotus/kem_lotus.c
	$(CC) -c src/kem/lotus/kem_lotus.c -o .objs/kem/lotus/kem_lotus.o $(CFLAGS)

ifneq (,$(findstring LOTUS192KEM, $(ENABLE_KEMS)))
UPSTREAMS+=LOTUS192KEM_upstream
endif

LOTUS192KEM_DIR=src/kem/lotus/upstream/Optimized_Implementation/kem/lotus192

SRCS_KEM_LOTUS192KEM=$(LOTUS192KEM_DIR)/lwe-arithmetics_opt.c \
                     $(LOTUS192KEM_DIR)/crypto.c \
                     $(LOTUS192KEM_DIR)/sampler.c \
                     $(LOTUS192KEM_DIR)/pack.c \
                     $(LOTUS192KEM_DIR)/kem.c \
                     $(LOTUS192KEM_DIR)/cpa-pke_opt.c

OBJS_KEM_LOTUS192KEM=$(SRCS_KEM_LOTUS192KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_LOTUS192KEM)

src/kem/lotus/upstream/Optimized_Implementation/kem/lotus192/%.o: src/kem/lotus/upstream/Optimized_Implementation/kem/lotus192/%.c
	$(CC) -O2 -fPIC -c -o $@ $<

MODULE_LOTUS192KEM=kem_LOTUS192KEM

LOTUS192KEM_upstream: $(OBJS_KEM_LOTUS192KEM)
	bash scripts/collect_objects.sh $(MODULE_LOTUS192KEM) $(OBJS_KEM_LOTUS192KEM)
	bash scripts/symbols_global_rename.sh $(MODULE_LOTUS192KEM) src/kem/lotus/symbols_global_rename_LOTUS192KEM.txt
	bash scripts/symbols_local.sh $(MODULE_LOTUS192KEM) src/kem/lotus/symbols_local.txt

ifneq (,$(findstring LOTUS256KEM, $(ENABLE_KEMS)))
UPSTREAMS+=LOTUS256KEM_upstream
endif

LOTUS256KEM_DIR=src/kem/lotus/upstream/Optimized_Implementation/kem/lotus256

SRCS_KEM_LOTUS256KEM=$(LOTUS256KEM_DIR)/lwe-arithmetics_opt.c \
                     $(LOTUS256KEM_DIR)/crypto.c \
                     $(LOTUS256KEM_DIR)/sampler.c \
                     $(LOTUS256KEM_DIR)/pack.c \
                     $(LOTUS256KEM_DIR)/kem.c \
                     $(LOTUS256KEM_DIR)/cpa-pke_opt.c

OBJS_KEM_LOTUS256KEM=$(SRCS_KEM_LOTUS256KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_LOTUS256KEM)

src/kem/lotus/upstream/Optimized_Implementation/kem/lotus256/%.o: src/kem/lotus/upstream/Optimized_Implementation/kem/lotus256/%.c
	$(CC) -O2 -fPIC -c -o $@ $<

MODULE_LOTUS256KEM=kem_LOTUS256KEM

LOTUS256KEM_upstream: $(OBJS_KEM_LOTUS256KEM)
	bash scripts/collect_objects.sh $(MODULE_LOTUS256KEM) $(OBJS_KEM_LOTUS256KEM)
	bash scripts/symbols_global_rename.sh $(MODULE_LOTUS256KEM) src/kem/lotus/symbols_global_rename_LOTUS256KEM.txt
	bash scripts/symbols_local.sh $(MODULE_LOTUS256KEM) src/kem/lotus/symbols_local.txt

ifneq (,$(findstring LOTUS128KEM, $(ENABLE_KEMS)))
UPSTREAMS+=LOTUS128KEM_upstream
endif

LOTUS128KEM_DIR=src/kem/lotus/upstream/Optimized_Implementation/kem/lotus128

SRCS_KEM_LOTUS128KEM=$(LOTUS128KEM_DIR)/lwe-arithmetics_opt.c \
                     $(LOTUS128KEM_DIR)/crypto.c \
                     $(LOTUS128KEM_DIR)/sampler.c \
                     $(LOTUS128KEM_DIR)/pack.c \
                     $(LOTUS128KEM_DIR)/kem.c \
                     $(LOTUS128KEM_DIR)/cpa-pke_opt.c

OBJS_KEM_LOTUS128KEM=$(SRCS_KEM_LOTUS128KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_LOTUS128KEM)

src/kem/lotus/upstream/Optimized_Implementation/kem/lotus128/%.o: src/kem/lotus/upstream/Optimized_Implementation/kem/lotus128/%.c
	$(CC) -O2 -fPIC -c -o $@ $<

MODULE_LOTUS128KEM=kem_LOTUS128KEM

LOTUS128KEM_upstream: $(OBJS_KEM_LOTUS128KEM)
	bash scripts/collect_objects.sh $(MODULE_LOTUS128KEM) $(OBJS_KEM_LOTUS128KEM)
	bash scripts/symbols_global_rename.sh $(MODULE_LOTUS128KEM) src/kem/lotus/symbols_global_rename_LOTUS128KEM.txt
	bash scripts/symbols_local.sh $(MODULE_LOTUS128KEM) src/kem/lotus/symbols_local.txt
