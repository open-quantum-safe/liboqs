ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_TITANIUM=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
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

ifneq (,$(findstring titanium_cca_std_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_std_kem_upstream
endif

BASEDIR=src/kem/titanium/upstream/Titanium_CCA_std
SRCS_KEM_TITANIUM_CCA_STD=$(BASEDIR)/encrypt.c \
						$(BASEDIR)/kem.c \
						$(BASEDIR)/ntt.c \
						$(BASEDIR)/pack.c \
						$(BASEDIR)/sampler.c \
						$(BASEDIR)/fastrandombytes.c \
						$(BASEDIR)/rng.c

OBJS_KEM_TITANIUM_CCA_STD=$(SRCS_KEM_TITANIUM_CCA_STD:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_STD) $(OBJS_KEM_TITANIUM)

src/kem/titanium/upstream/Titanium_CCA_std/%.o: src/kem/titanium/upstream/Titanium_CCA_std/%.c $(BASEDIR)/littleendian.h $(BASEDIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

titanium_cca_std_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_STD)
	bash scripts/collect_objects.sh titanium $(BASEDIR)/*.o
	bash scripts/symbols_global_rename.sh titanium    src/kem/titanium/symbols_global_rename_std.txt
	bash scripts/symbols_local.sh titanium     src/kem/titanium/symbols_local.txt
