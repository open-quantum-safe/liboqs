ENABLE_KEMS += $(findstring bike1_l1, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike1_l3, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike1_l5, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike2_l1, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike2_l3, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike2_l5, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l1, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l3, $(KEMS_TO_ENABLE))
ENABLE_KEMS += $(findstring bike3_l5, $(KEMS_TO_ENABLE))

HEADERS_KEM_BIKE = src/kem/bike/kem_bike.h
HEADERS_KEM += $(HEADERS_KEM_BIKE)

OBJECT_DIRS+=.objs/kem/bike
OBJECTS_KEM_BIKE=.objs/kem/bike/kem_bike.o
OBJECTS_KEM+=$(OBJECTS_KEM_BIKE)

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

ifneq (,$(findstring cc, $(CC)))
  ifneq (,$(AVX512_SUPPORT))
    KEM_BIKE_UPSTREAM_DIR := src/kem/bike/upstream/additional_implementation
  else ifneq (,$(AVX2_SUPPORT))
    KEM_BIKE_UPSTREAM_DIR := src/kem/bike/upstream/additional_implementation
  endif
endif

KEM_BIKE_UPSTREAM_DIR ?= src/kem/bike/upstream/reference_implementation

.objs/kem/bike/kem_bike.o: headers src/kem/bike/kem_bike.c
	$(CC) -c src/kem/bike/kem_bike.c -o .objs/kem/bike/kem_bike.o $(CFLAGS)

kem_bike1_l1_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike1_l1
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike1_l1
	cd .objs_upstream/kem_bike1_l1 && make clean && make VER=1 LEVEL=1 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike1_l1 .objs_upstream/kem_bike1_l1/*.o
	bash scripts/symbols_global_rename.sh kem_bike1_l1 src/kem/bike/symbols_global_rename_bike1_l1.txt
	bash scripts/symbols_local.sh kem_bike1_l1 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike1_l1

kem_bike1_l3_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike1_l3
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike1_l3
	cd .objs_upstream/kem_bike1_l3 && make clean && make VER=1 LEVEL=3 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike1_l3 .objs_upstream/kem_bike1_l3/*.o
	bash scripts/symbols_global_rename.sh kem_bike1_l3 src/kem/bike/symbols_global_rename_bike1_l3.txt
	bash scripts/symbols_local.sh kem_bike1_l3 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike1_l3

kem_bike1_l5_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike1_l5
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike1_l5
	cd .objs_upstream/kem_bike1_l5 && make clean && make VER=1 LEVEL=5 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike1_l5 .objs_upstream/kem_bike1_l5/*.o
	bash scripts/symbols_global_rename.sh kem_bike1_l5 src/kem/bike/symbols_global_rename_bike1_l5.txt
	bash scripts/symbols_local.sh kem_bike1_l5 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike1_l5

kem_bike2_l1_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike2_l1
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike2_l1
	cd .objs_upstream/kem_bike2_l1 && make clean && make VER=2 LEVEL=1 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike2_l1 .objs_upstream/kem_bike2_l1/*.o
	bash scripts/symbols_global_rename.sh kem_bike2_l1 src/kem/bike/symbols_global_rename_bike2_l1.txt
	bash scripts/symbols_local.sh kem_bike2_l1 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike2_l1

kem_bike2_l3_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike2_l3
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike2_l3
	cd .objs_upstream/kem_bike2_l3 && make clean && make VER=2 LEVEL=3 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike2_l3 .objs_upstream/kem_bike2_l3/*.o
	bash scripts/symbols_global_rename.sh kem_bike2_l3 src/kem/bike/symbols_global_rename_bike2_l3.txt
	bash scripts/symbols_local.sh kem_bike2_l3 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike2_l3

kem_bike2_l5_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike2_l5
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike2_l5
	cd .objs_upstream/kem_bike2_l5 && make clean && make VER=2 LEVEL=5 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike2_l5 .objs_upstream/kem_bike2_l5/*.o
	bash scripts/symbols_global_rename.sh kem_bike2_l5 src/kem/bike/symbols_global_rename_bike2_l5.txt
	bash scripts/symbols_local.sh kem_bike2_l5 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike2_l5

kem_bike3_l1_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike3_l1
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike3_l1
	cd .objs_upstream/kem_bike3_l1 && make clean && make VER=3 LEVEL=1 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike3_l1 .objs_upstream/kem_bike3_l1/*.o
	bash scripts/symbols_global_rename.sh kem_bike3_l1 src/kem/bike/symbols_global_rename_bike3_l1.txt
	bash scripts/symbols_local.sh kem_bike3_l1 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike3_l1

kem_bike3_l3_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike3_l3
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike3_l3
	cd .objs_upstream/kem_bike3_l3 && make clean && make VER=3 LEVEL=3 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike3_l3 .objs_upstream/kem_bike3_l3/*.o
	bash scripts/symbols_global_rename.sh kem_bike3_l3 src/kem/bike/symbols_global_rename_bike3_l3.txt
	bash scripts/symbols_local.sh kem_bike3_l3 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike3_l3

kem_bike3_l5_upstream: mkdirs headers
	$(RM) -r .objs_upstream/kem_bike3_l5
	cp -pr $(KEM_BIKE_UPSTREAM_DIR) .objs_upstream/kem_bike3_l5
	cd .objs_upstream/kem_bike3_l5 && make clean && make VER=3 LEVEL=5 CFLAGS="$(CFLAGS)" OPENSSL_INCLUDE_DIR=$(OPENSSL_INCLUDE_DIR) INC_DIR="$(realpath include)"
	bash scripts/collect_objects.sh kem_bike3_l5 .objs_upstream/kem_bike3_l5/*.o
	bash scripts/symbols_global_rename.sh kem_bike3_l5 src/kem/bike/symbols_global_rename_bike3_l5.txt
	bash scripts/symbols_local.sh kem_bike3_l5 src/kem/bike/symbols_local.txt
	$(RM) -r .objs_upstream/kem_bike3_l5
