ifeq (x64,$(ARCH))
ENABLE_SIGS+=$(findstring picnic_L1_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L1_UR, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L3_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L3_UR, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L5_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L5_UR, $(SIGS_TO_ENABLE))
MAKE_FLAGS_SIG_PICNIC=ARCH=x64
else ifeq (x86,$(ARCH))
ENABLE_SIGS+=$(findstring picnic_L1_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L1_UR, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L3_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L3_UR, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L5_FS, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring picnic_L5_UR, $(SIGS_TO_ENABLE))
MAKE_FLAGS_SIG_PICNIC=ARCH=x86
endif

HEADERS_SIG_PICNIC=src/sig/picnic/sig_picnic.h
HEADERS_SIG+=$(HEADERS_SIG_PICNIC)

OBJECT_DIRS+=.objs/sig/picnic
OBJECTS_SIG_PICNIC=.objs/sig/picnic/sig_picnic.o
OBJECTS_SIG+=$(OBJECTS_SIG_PICNIC)

TO_CLEAN+= $(OBJECTS_SIG_PICNIC)

.objs/sig/picnic/sig_picnic.o: headers src/sig/picnic/sig_picnic.c
	$(CC) -c src/sig/picnic/sig_picnic.c -o .objs/sig/picnic/sig_picnic.o $(CFLAGS)

ifneq (,$(findstring picnic_L1_FS, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L1_FS
endif

ifneq (,$(findstring picnic_L1_UR, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L1_UR
endif

ifneq (,$(findstring picnic_L3_FS, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L3_FS
endif

ifneq (,$(findstring picnic_L3_UR, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L3_UR
endif

ifneq (,$(findstring picnic_L5_FS, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L5_FS
endif

ifneq (,$(findstring picnic_L5_UR, $(ENABLE_SIGS)))
UPSTREAMS+=sig_picnic_L5_UR
endif

sig_picnic_L1_FS:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl1fs && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L1_FS src/sig/picnic/upstream/Optimized_Implementation/picnicl1fs/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L1_FS src/sig/picnic/symbols_global_rename_L1_FS.txt
	bash scripts/symbols_local.sh sig_picnic_L1_FS src/sig/picnic/symbols_local.txt

sig_picnic_L1_UR:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl1ur && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L1_UR src/sig/picnic/upstream/Optimized_Implementation/picnicl1ur/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L1_UR src/sig/picnic/symbols_global_rename_L1_UR.txt
	bash scripts/symbols_local.sh sig_picnic_L1_UR src/sig/picnic/symbols_local.txt

sig_picnic_L3_FS:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl3fs && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L3_FS src/sig/picnic/upstream/Optimized_Implementation/picnicl3fs/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L3_FS src/sig/picnic/symbols_global_rename_L3_FS.txt
	bash scripts/symbols_local.sh sig_picnic_L3_FS src/sig/picnic/symbols_local.txt

sig_picnic_L3_UR:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl3ur && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L3_UR src/sig/picnic/upstream/Optimized_Implementation/picnicl3ur/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L3_UR src/sig/picnic/symbols_global_rename_L3_UR.txt
	bash scripts/symbols_local.sh sig_picnic_L3_UR src/sig/picnic/symbols_local.txt

sig_picnic_L5_FS:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl5fs && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L5_FS src/sig/picnic/upstream/Optimized_Implementation/picnicl5fs/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L5_FS src/sig/picnic/symbols_global_rename_L5_FS.txt
	bash scripts/symbols_local.sh sig_picnic_L5_FS src/sig/picnic/symbols_local.txt

sig_picnic_L5_UR:
	cd src/sig/picnic/upstream/Optimized_Implementation/picnicl5ur && $(MAKE) clean && $(MAKE) $(MAKE_FLAGS_SIG_PICNIC) libpicnic.a
	bash scripts/explode_and_collect.sh sig_picnic_L5_UR src/sig/picnic/upstream/Optimized_Implementation/picnicl5ur/libpicnic.a
	bash scripts/symbols_global_rename.sh sig_picnic_L5_UR src/sig/picnic/symbols_global_rename_L5_UR.txt
	bash scripts/symbols_local.sh sig_picnic_L5_UR src/sig/picnic/symbols_local.txt
