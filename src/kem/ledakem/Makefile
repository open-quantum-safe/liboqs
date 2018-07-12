ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring ledakem_C1_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C1_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C1_N04, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N04, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N04, ${KEMS_TO_ENABLE})
MAKE_FLAGS_KEM_LEDAKEM=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring ledakem_C1_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C1_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C1_N04, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C3_N04, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N02, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N03, ${KEMS_TO_ENABLE})
ENABLE_KEMS+=$(findstring ledakem_C5_N04, ${KEMS_TO_ENABLE})
MAKE_FLAGS_KEM_LEDAKEM=ARCH=x86
endif

HEADERS_KEM_LEDAKEM=src/kem/ledakem/kem_LEDAkem.h
HEADERS_KEM+=$(HEADERS_KEM_LEDAKEM)

OBJECT_DIRS+=.objs/kem/ledakem
OBJECTS_KEM_LEDAKEM=.objs/kem/ledakem/kem_LEDAkem.o
OBJECTS_KEM+=$(OBJECTS_KEM_LEDAKEM)

.objs/kem/ledakem/kem_LEDAkem.o: src/kem/ledakem/kem_LEDAkem.c
	$(CC) -c src/kem/ledakem/kem_LEDAkem.c -o .objs/kem/ledakem/kem_LEDAkem.o $(CFLAGS)


ifneq (,$(findstring ledakem_C1_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N02
endif

kem_ledakem_C1_N02:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=1 && export N0=2 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl1_N02.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl1_N02.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N02  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl1_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_global_rename_C1_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C1_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N03
endif

kem_ledakem_C1_N03:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=1 && export N0=3 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl1_N03.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl1_N03.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N03  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl1_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_global_rename_C1_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C1_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N04
endif

kem_ledakem_C1_N04:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=1 && export N0=4 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl1_N04.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl1_N04.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N04  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl1_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_global_rename_C1_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C3_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N02
endif

kem_ledakem_C3_N02:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=3 && export N0=2 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl3_N02.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl3_N02.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N02  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl3_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_global_rename_C3_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C3_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N03
endif

kem_ledakem_C3_N03:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=3 && export N0=3 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl3_N03.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl3_N03.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N03  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl3_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_global_rename_C3_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C3_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N04
endif

kem_ledakem_C3_N04:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=3 && export N0=4 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl3_N04.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl3_N04.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N04  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl3_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_global_rename_C3_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C5_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N02
endif

kem_ledakem_C5_N02:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=5 && export N0=2 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl5_N02.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl5_N02.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N02  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl5_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_global_rename_C5_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C5_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N03
endif

kem_ledakem_C5_N03:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=5 && export N0=3 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl5_N03.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl5_N03.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N03  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl5_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_global_rename_C5_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 

ifneq (,$(findstring ledakem_C5_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N04
endif

kem_ledakem_C5_N04:
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && export SL=5 && export N0=4 && $(MAKE) clean && $(MAKE) ${MAKE_FLAGS_KEM_LEDAKEM} 
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/ && ar xv libLEDAkem_sl5_N04.a && strip -N initialize_pseudo_random_generator_seed rng.o && strip -N randombytes rng.o \
	&& strip -N randombytes_init rng.o && strip -N deterministic_random_byte_generator rng.o && ar rcs libLEDAkem_sl5_N04.a *.o
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N04  src/kem/ledakem/upstream/Reference_Implementation/KEM/bin/libLEDAkem_sl5_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_global_rename_C5_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_local.txt
	cd src/kem/ledakem/upstream/Reference_Implementation/KEM && rm -rf bin/* 



