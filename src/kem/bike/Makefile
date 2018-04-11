ENABLE_KEMS += $(findstring bike1_l1, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike1_l3, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike1_l5, $(KEMS_TO_ENABLE))
#ENABLE_KEMS += $(findstring bike2_l1, $(KEMS_TO_ENABLE))
#ENABLE_KEMS += $(findstring bike2_l3, $(KEMS_TO_ENABLE))
#ENABLE_KEMS += $(findstring bike2_l5, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l1, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l3, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l5, $(KEMS_TO_ENABLE))

HEADERS_KEM_BIKE = src/kem/bike/kem_bike.h
HEADERS_KEM += $(HEADERS_KEM_BIKE)

OBJECT_DIRS+=.objs/kem/bike
OBJECTS_KEM_BIKE=.objs/kem/bike/kem_bike.o
OBJECTS_KEM+=$(OBJECTS_KEM_BIKE)

.objs/kem/bike/kem_bike.o: src/kem/bike/kem_bike.c
	$(CC) -c src/kem/bike/kem_bike.c -o .objs/kem/bike/kem_bike.o $(CFLAGS)

ifneq (,$(findstring bike1_l1, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike1_l1_upstream
endif

ifneq (,$(findstring bike1_l3, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike1_l3_upstream
endif

ifneq (,$(findstring bike1_l5, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike1_l5_upstream
endif

ifneq (,$(findstring bike2_l1, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike2_l1_upstream
endif

ifneq (,$(findstring bike2_l3, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike2_l3_upstream
endif

ifneq (,$(findstring bike2_l5, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike2_l5_upstream
endif

ifneq (,$(findstring bike3_l1, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike3_l1_upstream
endif

ifneq (,$(findstring bike3_l3, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike3_l3_upstream
endif

ifneq (,$(findstring bike3_l5, $(ENABLE_KEMS)))
  UPSTREAMS+=kem_bike3_l5_upstream
endif

UPSTREAM_DIR := src/kem/bike/upstream
OBJS_KEM_BIKE = $(UPSTREAM_DIR)/*.o

TO_CLEAN += $(OBJS_KEM_BIKE)

kem_bike1_l1_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=1 LEVEL=1 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike1_l1 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike1_l1 src/kem/bike/symbols_global_rename_bike1_l1.txt
	bash scripts/symbols_local.sh kem_bike1_l1 src/kem/bike/symbols_local.txt

kem_bike1_l3_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=1 LEVEL=3 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike1_l3 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike1_l3 src/kem/bike/symbols_global_rename_bike1_l3.txt
	bash scripts/symbols_local.sh kem_bike1_l3 src/kem/bike/symbols_local.txt

kem_bike1_l5_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=1 LEVEL=5 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike1_l5 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike1_l5 src/kem/bike/symbols_global_rename_bike1_l5.txt
	bash scripts/symbols_local.sh kem_bike1_l5 src/kem/bike/symbols_local.txt

kem_bike2_l1_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=2 LEVEL=1 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike2_l1 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike2_l1 src/kem/bike/symbols_global_rename_bike2_l1.txt
	bash scripts/symbols_local.sh kem_bike2_l1 src/kem/bike/symbols_local.txt

kem_bike2_l3_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=2 LEVEL=3 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike2_l3 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike2_l3 src/kem/bike/symbols_global_rename_bike2_l3.txt
	bash scripts/symbols_local.sh kem_bike2_l3 src/kem/bike/symbols_local.txt

kem_bike2_l5_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=2 LEVEL=5 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike2_l5 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike2_l5 src/kem/bike/symbols_global_rename_bike2_l5.txt
	bash scripts/symbols_local.sh kem_bike2_l5 src/kem/bike/symbols_local.txt

kem_bike3_l1_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=3 LEVEL=1 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike3_l1 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike3_l1 src/kem/bike/symbols_global_rename_bike3_l1.txt
	bash scripts/symbols_local.sh kem_bike3_l1 src/kem/bike/symbols_local.txt

kem_bike3_l3_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=3 LEVEL=3 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike3_l3 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike3_l3 src/kem/bike/symbols_global_rename_bike3_l3.txt
	bash scripts/symbols_local.sh kem_bike3_l3 src/kem/bike/symbols_local.txt

kem_bike3_l5_upstream: 
	cd $(UPSTREAM_DIR) && make clean && make VER=3 LEVEL=5 OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR=$(realpath include)
	bash scripts/collect_objects.sh kem_bike3_l5 $(OBJS_KEM_BIKE)
	bash scripts/symbols_global_rename.sh kem_bike3_l5 src/kem/bike/symbols_global_rename_bike3_l5.txt
	bash scripts/symbols_local.sh kem_bike3_l5 src/kem/bike/symbols_local.txt
