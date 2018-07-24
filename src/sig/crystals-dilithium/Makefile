ENABLE_SIGS+=$(findstring Dilithium_II_medium, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring Dilithium_III_recommended, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring Dilithium_IV_very_high, $(SIGS_TO_ENABLE))

HEADERS_SIG_DILITHIUM=src/sig/crystals-dilithium/sig_dilithium.h
HEADERS_SIG+=$(HEADERS_SIG_DILITHIUM)

OBJECT_DIRS+=.objs/sig/crystals-dilithium
OBJECTS_SIG_DILITHIUM=.objs/sig/crystals-dilithium/sig_dilithium.o
OBJECTS_SIG+=$(OBJECTS_SIG_DILITHIUM)

.objs/sig/crystals-dilithium/sig_dilithium.o: headers src/sig/crystals-dilithium/sig_dilithium.c
	$(CC) -c src/sig/crystals-dilithium/sig_dilithium.c -o .objs/sig/crystals-dilithium/sig_dilithium.o $(CFLAGS)

ifneq (,$(findstring Dilithium_II_medium, $(ENABLE_SIGS)))
UPSTREAMS+=sig_Dilithium_II_medium_upstream
endif

SRCS_SIG_DILITHIUM_II_MEDIUM=src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/sign.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/rounding.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/reduce.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/polyvec.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/poly.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/packing.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/ntt.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/fips202.c
OBJS_SIG_DILITHIUM_II_MEDIUM=$(SRCS_SIG_DILITHIUM_II_MEDIUM:.c=.o)

TO_CLEAN+= $(OBJS_SIG_DILITHIUM_II_MEDIUM)

src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/%.o: src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_medium/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $<

sig_Dilithium_II_medium_upstream: $(OBJS_SIG_DILITHIUM_II_MEDIUM)
	bash scripts/collect_objects.sh sig_Dilithium_II_medium $(OBJS_SIG_DILITHIUM_II_MEDIUM)
	bash scripts/symbols_global_rename.sh sig_Dilithium_II_medium src/sig/crystals-dilithium/symbols_global_rename_II.txt
	bash scripts/symbols_local.sh sig_Dilithium_II_medium src/sig/crystals-dilithium/symbols_local.txt




ifneq (,$(findstring Dilithium_III_recommended, $(ENABLE_SIGS)))
UPSTREAMS+=sig_Dilithium_III_recommended_upstream
endif

SRCS_SIG_DILITHIUM_III_RECOMMENDED=src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/sign.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/rounding.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/reduce.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/polyvec.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/poly.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/packing.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/ntt.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/fips202.c
OBJS_SIG_DILITHIUM_III_RECOMMENDED=$(SRCS_SIG_DILITHIUM_III_RECOMMENDED:.c=.o)

TO_CLEAN+= $(OBJS_SIG_DILITHIUM_III_RECOMMENDED)

src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/%.o: src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_recommended/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $<

sig_Dilithium_III_recommended_upstream: $(OBJS_SIG_DILITHIUM_III_RECOMMENDED)
	bash scripts/collect_objects.sh sig_Dilithium_III_recommended $(OBJS_SIG_DILITHIUM_III_RECOMMENDED)
	bash scripts/symbols_global_rename.sh sig_Dilithium_III_recommended src/sig/crystals-dilithium/symbols_global_rename_III.txt
	bash scripts/symbols_local.sh sig_Dilithium_III_recommended src/sig/crystals-dilithium/symbols_local.txt



ifneq (,$(findstring Dilithium_IV_very_high, $(ENABLE_SIGS)))
UPSTREAMS+=sig_Dilithium_IV_very_high_upstream
endif

SRCS_SIG_DILITHIUM_IV_VERY_HIGH=src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/sign.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/rounding.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/reduce.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/polyvec.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/poly.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/packing.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/ntt.c src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/fips202.c
OBJS_SIG_DILITHIUM_IV_VERY_HIGH=$(SRCS_SIG_DILITHIUM_IV_VERY_HIGH:.c=.o)

TO_CLEAN+= $(OBJS_SIG_DILITHIUM_IV_VERY_HIGH)

src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/%.o: src/sig/crystals-dilithium/upstream/Reference_implementation/Dilithium_very_high/%.c
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $<

sig_Dilithium_IV_very_high_upstream: $(OBJS_SIG_DILITHIUM_IV_VERY_HIGH)
	bash scripts/collect_objects.sh sig_Dilithium_IV_very_high $(OBJS_SIG_DILITHIUM_IV_VERY_HIGH)
	bash scripts/symbols_global_rename.sh sig_Dilithium_IV_very_high src/sig/crystals-dilithium/symbols_global_rename_IV.txt
	bash scripts/symbols_local.sh sig_Dilithium_IV_very_high src/sig/crystals-dilithium/symbols_local.txt
