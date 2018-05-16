ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring saber_light_saber_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring saber_saber_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring saber_fire_saber_kem, $(KEMS_TO_ENABLE))
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring saber_light_saber_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring saber_saber_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring saber_fire_saber_kem, $(KEMS_TO_ENABLE))
endif

HEADERS_KEM_SABER=src/kem/saber/kem_saber.h
HEADERS_KEM+=$(HEADERS_KEM_SABER)

OBJECT_DIRS+=.objs/kem/saber
OBJECTS_KEM_SABER=.objs/kem/saber/kem_saber.o
OBJECTS_KEM+=$(OBJECTS_KEM_SABER)

.objs/kem/saber/kem_saber.o: headers src/kem/saber/kem_saber.c
	$(CC) -c src/kem/saber/kem_saber.c -o .objs/kem/saber/kem_saber.o $(CFLAGS)

ifneq (,$(findstring saber_light_saber_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_saber_light_saber_kem_upstream
endif

SRCS_KEM_SABER_LIGHT_SABER_KEM= src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/pack_unpack.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/poly.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/fips202.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/verify.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/recon.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/cbd.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/SABER_indcpa.c src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/kem.c
OBJS_KEM_SABER_LIGHT_SABER_KEM=$(SRCS_KEM_SABER_LIGHT_SABER_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_SABER_LIGHT_SABER_KEM)

src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/%.o: src/kem/saber/upstream/Optimized_Implementation_KEM/light_saber/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_saber_light_saber_kem_upstream: $(OBJS_KEM_SABER_LIGHT_SABER_KEM)
	bash scripts/collect_objects.sh kem_saber_light_saber_kem $(OBJS_KEM_SABER_LIGHT_SABER_KEM)
	bash scripts/symbols_global_rename.sh kem_saber_light_saber_kem src/kem/saber/symbols_global_rename_light_saber.txt
	bash scripts/symbols_local.sh kem_saber_light_saber_kem src/kem/saber/symbols_local.txt

ifneq (,$(findstring saber_saber_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_saber_saber_kem_upstream
endif

SRCS_KEM_SABER_SABER_KEM= src/kem/saber/upstream/Optimized_Implementation_KEM/saber/pack_unpack.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/poly.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/fips202.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/verify.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/recon.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/cbd.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/SABER_indcpa.c src/kem/saber/upstream/Optimized_Implementation_KEM/saber/kem.c
OBJS_KEM_SABER_SABER_KEM=$(SRCS_KEM_SABER_SABER_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_SABER_SABER_KEM)

src/kem/saber/upstream/Optimized_Implementation_KEM/saber/%.o: src/kem/saber/upstream/Optimized_Implementation_KEM/saber/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_saber_saber_kem_upstream: $(OBJS_KEM_SABER_SABER_KEM)
	bash scripts/collect_objects.sh kem_saber_saber_kem $(OBJS_KEM_SABER_SABER_KEM)
	bash scripts/symbols_global_rename.sh kem_saber_saber_kem src/kem/saber/symbols_global_rename_saber.txt
	bash scripts/symbols_local.sh kem_saber_saber_kem src/kem/saber/symbols_local.txt

ifneq (,$(findstring saber_fire_saber_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_saber_fire_saber_kem_upstream
endif

SRCS_KEM_SABER_FIRE_SABER_KEM= src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/pack_unpack.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/poly.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/fips202.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/verify.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/recon.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/cbd.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/SABER_indcpa.c src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/kem.c
OBJS_KEM_SABER_FIRE_SABER_KEM=$(SRCS_KEM_SABER_FIRE_SABER_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_SABER_FIRE_SABER_KEM)

src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/%.o: src/kem/saber/upstream/Optimized_Implementation_KEM/fire_saber/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR)

kem_saber_fire_saber_kem_upstream: $(OBJS_KEM_SABER_FIRE_SABER_KEM)
	bash scripts/collect_objects.sh kem_saber_fire_saber_kem $(OBJS_KEM_SABER_FIRE_SABER_KEM)
	bash scripts/symbols_global_rename.sh kem_saber_fire_saber_kem src/kem/saber/symbols_global_rename_fire_saber.txt
	bash scripts/symbols_local.sh kem_saber_fire_saber_kem src/kem/saber/symbols_local.txt
