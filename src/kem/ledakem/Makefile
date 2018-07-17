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

HEADERS_KEM_LEDAKEM=src/kem/ledakem/kem_ledakem.h
HEADERS_KEM+=$(HEADERS_KEM_LEDAKEM)

OBJECT_DIRS+=.objs/kem/ledakem
OBJECTS_KEM_LEDAKEM=.objs/kem/ledakem/kem_ledakem.o
OBJECTS_KEM+=$(OBJECTS_KEM_LEDAKEM)

.objs/kem/ledakem/kem_ledakem.o: headers src/kem/ledakem/kem_ledakem.c
	$(CC) -c src/kem/ledakem/kem_ledakem.c -o .objs/kem/ledakem/kem_ledakem.o $(CFLAGS)

ifneq (,$(findstring ledakem_C1_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N02
TO_CLEAN+=.objs_upstream/ledakem_C1_N02
endif

kem_ledakem_C1_N02: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C1_N02
	cd .objs_upstream/ledakem_C1_N02 && export SL=1 && export N0=2 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N02  .objs_upstream/ledakem_C1_N02/bin/libLEDAkem_sl1_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_global_rename_C1_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C1_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N03
TO_CLEAN+=.objs_upstream/ledakem_C1_N03
endif

kem_ledakem_C1_N03: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C1_N03
	cd .objs_upstream/ledakem_C1_N03 && export SL=1 && export N0=3 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N03  .objs_upstream/ledakem_C1_N03/bin/libLEDAkem_sl1_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_global_rename_C1_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C1_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N04
TO_CLEAN+=.objs_upstream/ledakem_C1_N04
endif

kem_ledakem_C1_N04: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C1_N04
	cd .objs_upstream/ledakem_C1_N04 && export SL=1 && export N0=4 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N04  .objs_upstream/ledakem_C1_N04/bin/libLEDAkem_sl1_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_global_rename_C1_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C3_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N02
TO_CLEAN+=.objs_upstream/ledakem_C3_N02
endif

kem_ledakem_C3_N02: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C3_N02
	cd .objs_upstream/ledakem_C3_N02 && export SL=3 && export N0=2 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N02  .objs_upstream/ledakem_C3_N02/bin/libLEDAkem_sl3_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_global_rename_C3_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C3_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N03
TO_CLEAN+=.objs_upstream/ledakem_C3_N03
endif


kem_ledakem_C3_N03: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C3_N03
	cd .objs_upstream/ledakem_C3_N03 && export SL=3 && export N0=3 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N03   .objs_upstream/ledakem_C3_N03/bin/libLEDAkem_sl3_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_global_rename_C3_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C3_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N04
TO_CLEAN+=.objs_upstream/ledakem_C3_N04
endif

kem_ledakem_C3_N04: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C3_N04
	cd .objs_upstream/ledakem_C3_N04 && export SL=3 && export N0=4 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N04   .objs_upstream/ledakem_C3_N04/bin/libLEDAkem_sl3_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_global_rename_C3_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C5_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N02
TO_CLEAN+=.objs_upstream/ledakem_C5_N02
endif

kem_ledakem_C5_N02: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C5_N02
	cd .objs_upstream/ledakem_C5_N02 && export SL=5 && export N0=2 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N02  .objs_upstream/ledakem_C5_N02/bin/libLEDAkem_sl5_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_global_rename_C5_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C5_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N03
TO_CLEAN+=.objs_upstream/ledakem_C5_N03
endif

kem_ledakem_C5_N03: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C5_N03
	cd .objs_upstream/ledakem_C5_N03 && export SL=5 && export N0=3 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N03  .objs_upstream/ledakem_C5_N03/bin/libLEDAkem_sl5_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_global_rename_C5_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_local.txt

ifneq (,$(findstring ledakem_C5_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N04
TO_CLEAN+=.objs_upstream/ledakem_C5_N04
endif

kem_ledakem_C5_N04: mkdirs
	cp -pr src/kem/ledakem/upstream/Reference_Implementation/KEM/ .objs_upstream/ledakem_C5_N04
	cd .objs_upstream/ledakem_C5_N04 && export SL=5 && export N0=4 && $(MAKE) clean && $(MAKE)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N04  .objs_upstream/ledakem_C5_N04/bin/libLEDAkem_sl5_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_global_rename_C5_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_local.txt
