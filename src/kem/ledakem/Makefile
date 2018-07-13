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
CFLAGS+=-DCPU_WORD_BITS=64 -std=c99 -pedantic -Wmaybe-uninitialized -Wuninitialized -march=native -O3
SRCDIR=src/kem/ledakem/upstream/Reference_Implementation/KEM/library
INCLUDES=-Isrc/kem/ledakem/upstream/Reference_Implementation/KEM/include
CSRC = $(wildcard $(SRCDIR)/*.c) 

LEDAKEM_CFLAGS=-DCPU_WORD_BITS=64 -std=c99 -pedantic -Wmaybe-uninitialized -Wuninitialized -march=native -O3

.objs/kem/ledakem/kem_LEDAkem.o: src/kem/ledakem/kem_LEDAkem.c
	$(CC) -c src/kem/ledakem/kem_LEDAkem.c -o .objs/kem/ledakem/kem_LEDAkem.o $(CFLAGS)


ifneq (,$(findstring ledakem_C1_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N02
OBJDIR_C1N02=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C1N02
COBJS_C1N02=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C1N02)/%.o)
endif

$(OBJDIR_C1N02)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=1 -DN0=2 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C1N02:
	mkdir -p $(OBJDIR_C1N02)

kem_ledakem_C1_N02: MKDIR_C1N02 $(COBJS_C1N02)
	ar rcs $(OBJDIR_C1N02)/libLEDAkem_sl1_N02.a $(COBJS_C1N02)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N02  $(OBJDIR_C1N02)/libLEDAkem_sl1_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_global_rename_C1_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N02 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C1N02) 

ifneq (,$(findstring ledakem_C1_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N03
OBJDIR_C1N03=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C1N03
COBJS_C1N03=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C1N03)/%.o)
endif

$(OBJDIR_C1N03)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=1 -DN0=3 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C1N03:
	mkdir -p $(OBJDIR_C1N03)

kem_ledakem_C1_N03: MKDIR_C1N03 $(COBJS_C1N03)
	ar rcs $(OBJDIR_C1N03)/libLEDAkem_sl1_N03.a $(COBJS_C1N03)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N03  $(OBJDIR_C1N03)/libLEDAkem_sl1_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_global_rename_C1_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N03 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C1N03) 

ifneq (,$(findstring ledakem_C1_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C1_N04
OBJDIR_C1N04=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C1N04
COBJS_C1N04=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C1N04)/%.o)
endif

$(OBJDIR_C1N04)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=1 -DN0=4 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C1N04:
	mkdir -p $(OBJDIR_C1N04)

kem_ledakem_C1_N04: MKDIR_C1N04 $(COBJS_C1N04)
	ar rcs $(OBJDIR_C1N04)/libLEDAkem_sl1_N04.a $(COBJS_C1N04)
	bash scripts/explode_and_collect.sh kem_ledakem_C1_N04  $(OBJDIR_C1N04)/libLEDAkem_sl1_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_global_rename_C1_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C1_N04 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C1N04) 

ifneq (,$(findstring ledakem_C3_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N02
OBJDIR_C3N02=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C3N02
COBJS_C3N02=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C3N02)/%.o)
endif

$(OBJDIR_C3N02)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=3 -DN0=2 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C3N02:
	mkdir -p $(OBJDIR_C3N02)

kem_ledakem_C3_N02: MKDIR_C3N02 $(COBJS_C3N02) 
	ar rcs $(OBJDIR_C3N02)/libLEDAkem_sl3_N02.a $(COBJS_C3N02)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N02  $(OBJDIR_C3N02)/libLEDAkem_sl3_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_global_rename_C3_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N02 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C3N02) 

ifneq (,$(findstring ledakem_C3_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N03
OBJDIR_C3N03=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C3N03
COBJS_C3N03=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C3N03)/%.o)
endif

$(OBJDIR_C3N03)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=3 -DN0=3 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C3N03:
	mkdir -p $(OBJDIR_C3N03)

kem_ledakem_C3_N03: MKDIR_C3N03 $(COBJS_C3N03) 
	ar rcs $(OBJDIR_C3N03)/libLEDAkem_sl3_N03.a $(COBJS_C3N03)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N03  $(OBJDIR_C3N03)/libLEDAkem_sl3_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_global_rename_C3_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N03 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C3N03) 

ifneq (,$(findstring ledakem_C3_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C3_N04
OBJDIR_C3N04=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C3N04
COBJS_C3N04=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C3N04)/%.o)
endif

$(OBJDIR_C3N04)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=3 -DN0=4 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C3N04:
	mkdir -p $(OBJDIR_C3N04)

kem_ledakem_C3_N04: MKDIR_C3N04 $(COBJS_C3N04) 
	ar rcs $(OBJDIR_C3N04)/libLEDAkem_sl3_N04.a $(COBJS_C3N04)
	bash scripts/explode_and_collect.sh kem_ledakem_C3_N04  $(OBJDIR_C3N04)/libLEDAkem_sl3_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_global_rename_C3_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C3_N04 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C3N04) 

ifneq (,$(findstring ledakem_C5_N02, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N02
OBJDIR_C5N02=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C5N02
COBJS_C5N02=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C5N02)/%.o)
endif

$(OBJDIR_C5N02)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=5 -DN0=2 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C5N02:
	mkdir -p $(OBJDIR_C5N02)

kem_ledakem_C5_N02: MKDIR_C5N02 $(COBJS_C5N02) 
	ar rcs $(OBJDIR_C5N02)/libLEDAkem_sl5_N02.a $(COBJS_C5N02)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N02  $(OBJDIR_C5N02)/libLEDAkem_sl5_N02.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_global_rename_C5_N02.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N02 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C5N02) 

ifneq (,$(findstring ledakem_C5_N03, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N03
OBJDIR_C5N03=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C5N03
COBJS_C5N03=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C5N03)/%.o)
endif

$(OBJDIR_C5N03)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=5 -DN0=3 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C5N03:
	mkdir -p $(OBJDIR_C5N03)

kem_ledakem_C5_N03: MKDIR_C5N03 $(COBJS_C5N03) 
	ar rcs $(OBJDIR_C5N03)/libLEDAkem_sl5_N03.a $(COBJS_C5N03)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N03  $(OBJDIR_C5N03)/libLEDAkem_sl5_N03.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_global_rename_C5_N03.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N03 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C5N03) 

ifneq (,$(findstring ledakem_C5_N04, $(ENABLE_KEMS)))
UPSTREAMS+=kem_ledakem_C5_N04
OBJDIR_C5N04=src/kem/ledakem/upstream/Reference_Implementation/KEM/bin_C5N04
COBJS_C5N04=$(CSRC:$(SRCDIR)/%.c=$(OBJDIR_C5N04)/%.o)
endif

$(OBJDIR_C5N04)/%.o: $(SRCDIR)/%.c
	$(CC) -DCATEGORY=5 -DN0=4 $(INCLUDES) $(LEDAKEM_CFLAGS) -c  $< -o $@ 

MKDIR_C5N04:
	mkdir -p $(OBJDIR_C5N04)


kem_ledakem_C5_N04: MKDIR_C5N04 $(COBJS_C5N04) 
	ar rcs $(OBJDIR_C5N04)/libLEDAkem_sl5_N04.a $(COBJS_C5N04)
	bash scripts/explode_and_collect.sh kem_ledakem_C5_N04  $(OBJDIR_C5N04)/libLEDAkem_sl5_N04.a
	bash scripts/symbols_global_rename.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_global_rename_C5_N04.txt
	bash scripts/symbols_local.sh kem_ledakem_C5_N04 src/kem/ledakem/symbols_local.txt
	rm -rf $(OBJDIR_C5N04) 



