ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring frodokem_640_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_640_cshake, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_cshake, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_FRODOKEM=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring frodokem_640_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_640_cshake, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_aes, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring frodokem_976_cshake, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_FRODOKEM=ARCH=x86
endif

HEADERS_KEM_FRODOKEM=src/kem/frodokem/kem_frodokem.h
HEADERS_KEM+=$(HEADERS_KEM_FRODOKEM)

OBJECT_DIRS+=.objs/kem/frodokem
OBJECTS_KEM_FRODOKEM=.objs/kem/frodokem/kem_frodokem.o
OBJECTS_KEM+=$(OBJECTS_KEM_FRODOKEM)

.objs/kem/frodokem/kem_frodokem.o: src/kem/frodokem/kem_frodokem.c
	$(CC) -c src/kem/frodokem/kem_frodokem.c -o .objs/kem/frodokem/kem_frodokem.o $(CFLAGS)

ifneq (,$(findstring frodokem_640_aes, $(ENABLE_KEMS)))
UPSTREAMS+=kem_frodokem_640_aes
endif

ifneq (,$(findstring frodokem_976_aes, $(ENABLE_KEMS)))
UPSTREAMS+=kem_frodokem_976_aes
endif

ifneq (,$(findstring frodokem_640_cshake, $(ENABLE_KEMS)))
UPSTREAMS+=kem_frodokem_640_cshake
endif

ifneq (,$(findstring frodokem_976_cshake, $(ENABLE_KEMS)))
UPSTREAMS+=kem_frodokem_976_cshake
endif

kem_frodokem_640_aes:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=AES128 USE_OPENSSL=FALSE
	bash scripts/explode_and_collect.sh kem_frodokem_640_aes src/kem/frodokem/upstream/frodo640/libfrodo.a
	bash scripts/symbols_global_rename.sh kem_frodokem_640_aes src/kem/frodokem/symbols_global_rename_640_aes.txt
	bash scripts/symbols_local.sh kem_frodokem_640_aes src/kem/frodokem/symbols_local.txt

kem_frodokem_976_aes:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=AES128 USE_OPENSSL=FALSE
	bash scripts/explode_and_collect.sh kem_frodokem_976_aes src/kem/frodokem/upstream/frodo976/libfrodo.a
	bash scripts/symbols_global_rename.sh kem_frodokem_976_aes src/kem/frodokem/symbols_global_rename_976_aes.txt
	bash scripts/symbols_local.sh kem_frodokem_976_aes src/kem/frodokem/symbols_local.txt

kem_frodokem_640_cshake:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=CSHAKE128 USE_OPENSSL=FALSE
	bash scripts/explode_and_collect.sh kem_frodokem_640_cshake src/kem/frodokem/upstream/frodo640/libfrodo.a
	bash scripts/symbols_global_rename.sh kem_frodokem_640_cshake src/kem/frodokem/symbols_global_rename_640_cshake.txt
	bash scripts/symbols_local.sh kem_frodokem_640_cshake src/kem/frodokem/symbols_local.txt

kem_frodokem_976_cshake:
	cd src/kem/frodokem/upstream && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_KEM_FRODOKEM) OPT_LEVEL=FAST_GENERIC GENERATION_A=CSHAKE128 USE_OPENSSL=FALSE
	bash scripts/explode_and_collect.sh kem_frodokem_976_cshake src/kem/frodokem/upstream/frodo976/libfrodo.a
	bash scripts/symbols_global_rename.sh kem_frodokem_976_cshake src/kem/frodokem/symbols_global_rename_976_cshake.txt
	bash scripts/symbols_local.sh kem_frodokem_976_cshake src/kem/frodokem/symbols_local.txt
