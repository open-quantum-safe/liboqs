ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring sike_p503, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring sike_p751, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_SIKE=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring sike_p503, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring sike_p751, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_SIKE=ARCH=x86
endif

HEADERS_KEM_SIKE=src/kem/sike/kem_sike.h
HEADERS_KEM+=$(HEADERS_KEM_SIKE)

OBJECT_DIRS+=.objs/kem/sike
OBJECTS_KEM_SIKE=.objs/kem/sike/kem_sike.o
OBJECTS_KEM+=$(OBJECTS_KEM_SIKE)

TO_CLEAN+= $(OBJECTS_KEM_SIKE)

.objs/kem/sike/kem_sike.o: src/kem/sike/kem_sike.c
	$(CC) -c src/kem/sike/kem_sike.c -o .objs/kem/sike/kem_sike.o $(CFLAGS)

ifneq (,$(findstring sike_p503, $(ENABLE_KEMS)))
UPSTREAMS+=kem_sike_p503
endif

ifneq (,$(findstring sike_p751, $(ENABLE_KEMS)))
UPSTREAMS+=kem_sike_p751
endif

kem_sike_p503:
	cd src/kem/sike/upstream/Optimized_Implementation/portable/SIKEp503 && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_SIKE)
	bash scripts/explode_and_collect.sh kem_sike_p503 src/kem/sike/upstream/Optimized_Implementation/portable/SIKEp503/sike/libsike.a
	bash scripts/symbols_global_rename.sh kem_sike_p503 src/kem/sike/symbols_global_rename_p503.txt
	bash scripts/symbols_local.sh kem_sike_p503 src/kem/sike/symbols_local.txt

kem_sike_p751:
	cd src/kem/sike/upstream/Optimized_Implementation/portable/SIKEp751 && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_SIKE)
	bash scripts/explode_and_collect.sh kem_sike_p751 src/kem/sike/upstream/Optimized_Implementation/portable/SIKEp751/sike/libsike.a
	bash scripts/symbols_global_rename.sh kem_sike_p751 src/kem/sike/symbols_global_rename_p751.txt
	bash scripts/symbols_local.sh kem_sike_p751 src/kem/sike/symbols_local.txt
