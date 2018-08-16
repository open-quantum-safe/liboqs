ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_hi_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_med_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_super_kem, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_TITANIUM=
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring titanium_cca_std_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_hi_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_med_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring titanium_cca_super_kem, $(KEMS_TO_ENABLE))
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
ifneq (,$(findstring titanium_cca_std_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_std_kem_upstream
endif

STD_DIR=src/kem/titanium/upstream/Titanium_CCA_std
SRCS_KEM_TITANIUM_CCA_STD=$(STD_DIR)/encrypt.c \
						$(STD_DIR)/kem.c \
						$(STD_DIR)/ntt.c \
						$(STD_DIR)/pack.c \
						$(STD_DIR)/sampler.c \
						$(STD_DIR)/fastrandombytes.c

OBJS_KEM_TITANIUM_CCA_STD=$(SRCS_KEM_TITANIUM_CCA_STD:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_STD)

src/kem/titanium/upstream/Titanium_CCA_std/%.o: src/kem/titanium/upstream/Titanium_CCA_std/%.c $(STD_DIR)/littleendian.h $(STD_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

STD_MODULE=kem_titanium_cca_std

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
						$(HI_DIR)/fastrandombytes.c

OBJS_KEM_TITANIUM_CCA_HI=$(SRCS_KEM_TITANIUM_CCA_HI:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_HI)

src/kem/titanium/upstream/Titanium_CCA_hi/%.o: src/kem/titanium/upstream/Titanium_CCA_hi/%.c $(HI_DIR)/littleendian.h $(HI_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

HI_MODULE=kem_titanium_cca_hi

titanium_cca_hi_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_HI)
	bash scripts/collect_objects.sh $(HI_MODULE)  $(HI_DIR)/*.o
	bash scripts/symbols_global_rename.sh $(HI_MODULE)  src/kem/titanium/symbols_global_rename_hi.txt
	bash scripts/symbols_local.sh $(HI_MODULE)  src/kem/titanium/symbols_local.txt


# med strength make targets
ifneq (,$(findstring titanium_cca_med_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_med_kem_upstream
endif

MED_DIR=src/kem/titanium/upstream/Titanium_CCA_med
SRCS_KEM_TITANIUM_CCA_MED=$(MED_DIR)/encrypt.c \
						$(MED_DIR)/kem.c \
						$(MED_DIR)/ntt.c \
						$(MED_DIR)/pack.c \
						$(MED_DIR)/sampler.c \
						$(MED_DIR)/fastrandombytes.c

OBJS_KEM_TITANIUM_CCA_MED=$(SRCS_KEM_TITANIUM_CCA_MED:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_MED)

src/kem/titanium/upstream/Titanium_CCA_med/%.o: $(MED_DIR)/%.c $(MED_DIR)/littleendian.h $(MED_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

MED_MODULE=kem_titanium_cca_med

titanium_cca_med_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_MED)
	bash scripts/collect_objects.sh $(MED_MODULE)  $(MED_DIR)/*.o
	bash scripts/symbols_global_rename.sh $(MED_MODULE)  src/kem/titanium/symbols_global_rename_med.txt
	bash scripts/symbols_local.sh $(MED_MODULE)  src/kem/titanium/symbols_local.txt


# super strength make targets
ifneq (,$(findstring titanium_cca_super_kem, $(ENABLE_KEMS)))
UPSTREAMS+=titanium_cca_super_kem_upstream
endif

SUPER_DIR=src/kem/titanium/upstream/Titanium_CCA_super
SRCS_KEM_TITANIUM_CCA_SUPER=$(SUPER_DIR)/encrypt.c \
						$(SUPER_DIR)/kem.c \
						$(SUPER_DIR)/ntt.c \
						$(SUPER_DIR)/pack.c \
						$(SUPER_DIR)/sampler.c \
						$(SUPER_DIR)/fastrandombytes.c

OBJS_KEM_TITANIUM_CCA_SUPER=$(SRCS_KEM_TITANIUM_CCA_SUPER:.c=.o)

TO_CLEAN+=$(OBJS_KEM_TITANIUM_CCA_SUPER)

src/kem/titanium/upstream/Titanium_CCA_super/%.o: $(SUPER_DIR)/%.c $(SUPER_DIR)/littleendian.h $(SUPER_DIR)/fastmodulo.h
	$(CC) -O2 -fPIC -I$(KECCAK_INCLUDE_DIR) -c -o $@ $<

SUPER_MODULE=kem_titanium_cca_super

titanium_cca_super_kem_upstream: $(OBJS_KEM_TITANIUM_CCA_SUPER)
	bash scripts/collect_objects.sh $(SUPER_MODULE)  $(SUPER_DIR)/*.o
	bash scripts/symbols_global_rename.sh $(SUPER_MODULE)  src/kem/titanium/symbols_global_rename_super.txt
	bash scripts/symbols_local.sh $(SUPER_MODULE)  src/kem/titanium/symbols_local.txt
