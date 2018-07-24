ENABLE_SIGS+=$(findstring Dilithium_II_medium, $(SIGS_TO_ENABLE))
# ENABLE_SIGS+=$(findstring Dilithium_III_recommended, $(SIGS_TO_ENABLE))
# ENABLE_SIGS+=$(findstring Dilithium_IV_very_high, $(SIGS_TO_ENABLE))

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
