ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_hi_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_TITANIUM=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_hi_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_TITANIUM=
endif

HEADERS_KEM_TITANIUM=src/kem/titanium/kem_titanium.h
HEADERS_KEM+=$(HEADERS_KEM_TITANIUM)

OBJECT_DIRS+=.objs/kem/titanium
OBJECTS_KEM_TITANIUM=.objs/kem/titanium/kem_titanium.o
OBJECTS_KEM+=$(OBJECTS_KEM_TITANIUM)

# build liboqs 'interface' to upstream component
.objs/kem/titanium/kem_titanium.o: headers src/kem/titanium/kem_titanium.c
	$(CC) -c src/kem/titanium/kem_titanium.c -o .objs/kem/titanium/kem_titanium.o $(CFLAGS)

# Std strength make targets
STD_MODULE=kem_titanium_cca_std

ifneq (,$(findstring titanium_cca_std_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_std_kem_upstream
endif

STD_DIR=src/kem/titanium/upstream/Titanium_CCA_std
SRCS_KEM_TITANIUM_CCA_STD=$(STD_DIR)/encrypt.c \
						$(STD_DIR)/kem.c \
						$(STD_DIR)/ntt.c \
						$(STD_DIR)/pack.c \
						$(STD_DIR)/sampler.c \
						$(STD_DIR)/fastrandombytes.c \
						$(STD_DIR)/rng.c

OBJS_KEM_TITANIUM_CCA_STD=$(SRCS_KEM_TITANIUM_CCA_STD:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_STD)

src/kem/titanium/upstream/Titanium_CCA_std/%.o: src/kem/titanium/upstream/Titanium_CCA_std/%.c $(STD_DIR)/littleendian.h $(STD_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

titanium_cca_std_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_STD)
	bash scripts/collect_objects.sh $(STD_MODULE)  $(OBJS_KEM_TITANIUM_CCA_STD)
	bash scripts/symbols_global_rename.sh $(STD_MODULE)  src/kem/titanium/symbols_global_rename_std.txt
	bash scripts/symbols_local.sh $(STD_MODULE)  src/kem/titanium/symbols_local.txt


# hi strength make targets
ifneq (,$(findstring titanium_cca_hi_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_hi_kem_upstream
endif

HI_DIR=src/kem/titanium/upstream/Titanium_CCA_hi
SRCS_KEM_TITANIUM_CCA_HI=$(HI_DIR)/encrypt.c \
						$(HI_DIR)/kem.c \
						$(HI_DIR)/ntt.c \
						$(HI_DIR)/pack.c \
						$(HI_DIR)/sampler.c \
						$(HI_DIR)/fastrandombytes.c \
						$(HI_DIR)/rng.c

OBJS_KEM_TITANIUM_CCA_HI=$(SRCS_KEM_TITANIUM_CCA_HI:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_HI)

src/kem/titanium/upstream/Titanium_CCA_hi/%.o: src/kem/titanium/upstream/Titanium_CCA_hi/%.c $(HI_DIR)/littleendian.h $(HI_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

HI_MODULE=titanium_cca_hi

titanium_cca_hi_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_HI)
	bash scripts/collect_objects.sh $(HI_MODULE)  $(HI_DIR)/*.o
	bash scripts/symbols_global_rename.sh $(HI_MODULE)  src/kem/titanium/symbols_global_rename_hi.txt
	bash scripts/symbols_local.sh $(HI_MODULE)  src/kem/titanium/symbols_local.txt
