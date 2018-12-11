ENABLE_KEMS+=$(findstring babybear_cca, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring mamabear_cca, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring papabear_cca, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring babybear_ephem_cpa, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring mamabear_ephem_cpa, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring papabear_ephem_cpa, $(KEMS_TO_ENABLE))
MAKE_FLAGS_KEM_THREEBEARS=

HEADERS_KEM_THREEBEARS=src/kem/threebears/kem_threebears.h
HEADERS_KEM+=$(HEADERS_KEM_THREEBEARS)

OBJECT_DIRS+=.objs/kem/threebears
OBJECTS_KEM_THREEBEARS=.objs/kem/threebears/kem_threebears.o
OBJECTS_KEM+=$(OBJECTS_KEM_THREEBEARS)

.objs/kem/threebears/kem_threebears.o: libkeccak headers src/kem/threebears/kem_threebears.c
	$(CC) -c src/kem/threebears/kem_threebears.c -o .objs/kem/threebears/kem_threebears.o -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

ifneq (,$(findstring babybear_cca, $(ENABLE_KEMS)))
UPSTREAMS+=kem_babybear_cca_upstream
endif

SRCS_KEM_BABYBEAR_CCA_KEM=\
src/kem/threebears/upstream/BabyBear/ring.c \
src/kem/threebears/upstream/BabyBear/shake.c \
src/kem/threebears/upstream/BabyBear/kem.c \
src/kem/threebears/upstream/BabyBear/threebears.c \
src/kem/threebears/upstream/BabyBear/melas_fec.c
OBJS_KEM_BABYBEAR_CCA_KEM=$(SRCS_KEM_BABYBEAR_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_BABYBEAR_CCA_KEM)

src/kem/threebears/upstream/BabyBear/%.o: src/kem/threebears/upstream/BabyBear/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_babybear_cca_upstream: $(OBJS_KEM_BABYBEAR_CCA_KEM)
	bash scripts/collect_objects.sh kem_babybear_cca $(OBJS_KEM_BABYBEAR_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_babybear_cca src/kem/threebears/symbols_global_rename_babybear_cca.txt
	bash scripts/symbols_local.sh kem_babybear_cca src/kem/threebears/symbols_local.txt


ifneq (,$(findstring mamabear_cca, $(ENABLE_KEMS)))
UPSTREAMS+=kem_mamabear_cca_upstream
endif

SRCS_KEM_MAMABEAR_CCA_KEM=\
src/kem/threebears/upstream/MamaBear/ring.c \
src/kem/threebears/upstream/MamaBear/shake.c \
src/kem/threebears/upstream/MamaBear/kem.c \
src/kem/threebears/upstream/MamaBear/threebears.c \
src/kem/threebears/upstream/MamaBear/melas_fec.c
OBJS_KEM_MAMABEAR_CCA_KEM=$(SRCS_KEM_MAMABEAR_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_MAMABEAR_CCA_KEM)

src/kem/threebears/upstream/MamaBear/%.o: src/kem/threebears/upstream/MamaBear/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_mamabear_cca_upstream: $(OBJS_KEM_MAMABEAR_CCA_KEM)
	bash scripts/collect_objects.sh kem_mamabear_cca $(OBJS_KEM_MAMABEAR_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_mamabear_cca src/kem/threebears/symbols_global_rename_mamabear_cca.txt
	bash scripts/symbols_local.sh kem_mamabear_cca src/kem/threebears/symbols_local.txt


ifneq (,$(findstring papabear_cca, $(ENABLE_KEMS)))
UPSTREAMS+=kem_papabear_cca_upstream
endif

SRCS_KEM_PAPABEAR_CCA_KEM=\
src/kem/threebears/upstream/PapaBear/ring.c \
src/kem/threebears/upstream/PapaBear/shake.c \
src/kem/threebears/upstream/PapaBear/kem.c \
src/kem/threebears/upstream/PapaBear/threebears.c \
src/kem/threebears/upstream/PapaBear/melas_fec.c
OBJS_KEM_PAPABEAR_CCA_KEM=$(SRCS_KEM_PAPABEAR_CCA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_PAPABEAR_CCA_KEM)

src/kem/threebears/upstream/PapaBear/%.o: src/kem/threebears/upstream/PapaBear/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_papabear_cca_upstream: $(OBJS_KEM_PAPABEAR_CCA_KEM)
	bash scripts/collect_objects.sh kem_papabear_cca $(OBJS_KEM_PAPABEAR_CCA_KEM)
	bash scripts/symbols_global_rename.sh kem_papabear_cca src/kem/threebears/symbols_global_rename_papabear_cca.txt
	bash scripts/symbols_local.sh kem_papabear_cca src/kem/threebears/symbols_local.txt


ifneq (,$(findstring babybear_ephem_cpa, $(ENABLE_KEMS)))
UPSTREAMS+=kem_babybear_ephem_cpa_upstream
endif

SRCS_KEM_BABYBEAR_EPHEM_CPA_KEM=\
src/kem/threebears/upstream/BabyBearEphem/ring.c \
src/kem/threebears/upstream/BabyBearEphem/shake.c \
src/kem/threebears/upstream/BabyBearEphem/kem.c \
src/kem/threebears/upstream/BabyBearEphem/threebears.c \
src/kem/threebears/upstream/BabyBearEphem/melas_fec.c
OBJS_KEM_BABYBEAR_EPHEM_CPA_KEM=$(SRCS_KEM_BABYBEAR_EPHEM_CPA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_BABYBEAR_EPHEM_CPA_KEM)

src/kem/threebears/upstream/BabyBearEphem/%.o: src/kem/threebears/upstream/BabyBearEphem/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_babybear_ephem_cpa_upstream: $(OBJS_KEM_BABYBEAR_EPHEM_CPA_KEM)
	bash scripts/collect_objects.sh kem_babybear_ephem_cpa $(OBJS_KEM_BABYBEAR_EPHEM_CPA_KEM)
	bash scripts/symbols_global_rename.sh kem_babybear_ephem_cpa src/kem/threebears/symbols_global_rename_babybear_ephem_cpa.txt
	bash scripts/symbols_local.sh kem_babybear_ephem_cpa src/kem/threebears/symbols_local.txt


ifneq (,$(findstring mamabear_ephem_cpa, $(ENABLE_KEMS)))
UPSTREAMS+=kem_mamabear_ephem_cpa_upstream
endif

SRCS_KEM_MAMABEAR_EPHEM_CPA_KEM=\
src/kem/threebears/upstream/MamaBearEphem/ring.c \
src/kem/threebears/upstream/MamaBearEphem/shake.c \
src/kem/threebears/upstream/MamaBearEphem/kem.c \
src/kem/threebears/upstream/MamaBearEphem/threebears.c \
src/kem/threebears/upstream/MamaBearEphem/melas_fec.c
OBJS_KEM_MAMABEAR_EPHEM_CPA_KEM=$(SRCS_KEM_MAMABEAR_EPHEM_CPA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_MAMABEAR_EPHEM_CPA_KEM)

src/kem/threebears/upstream/MamaBearEphem/%.o: src/kem/threebears/upstream/MamaBearEphem/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_mamabear_ephem_cpa_upstream: $(OBJS_KEM_MAMABEAR_EPHEM_CPA_KEM)
	bash scripts/collect_objects.sh kem_mamabear_ephem_cpa $(OBJS_KEM_MAMABEAR_EPHEM_CPA_KEM)
	bash scripts/symbols_global_rename.sh kem_mamabear_ephem_cpa src/kem/threebears/symbols_global_rename_mamabear_ephem_cpa.txt
	bash scripts/symbols_local.sh kem_mamabear_ephem_cpa src/kem/threebears/symbols_local.txt


ifneq (,$(findstring papabear_ephem_cpa, $(ENABLE_KEMS)))
UPSTREAMS+=kem_papabear_ephem_cpa_upstream
endif

SRCS_KEM_PAPABEAR_EPHEM_CPA_KEM=\
src/kem/threebears/upstream/PapaBearEphem/ring.c \
src/kem/threebears/upstream/PapaBearEphem/shake.c \
src/kem/threebears/upstream/PapaBearEphem/kem.c \
src/kem/threebears/upstream/PapaBearEphem/threebears.c \
src/kem/threebears/upstream/PapaBearEphem/melas_fec.c
OBJS_KEM_PAPABEAR_EPHEM_CPA_KEM=$(SRCS_KEM_PAPABEAR_EPHEM_CPA_KEM:.c=.o)

TO_CLEAN+=$(OBJS_KEM_PAPABEAR_EPHEM_CPA_KEM)

src/kem/threebears/upstream/PapaBearEphem/%.o: src/kem/threebears/upstream/PapaBearEphem/%.c libkeccak
	$(CC) -c -fPIC -Os -std=c99 -march=native -o $@ $< -I $(KECCAK_INCLUDE_DIR) $(CFLAGS)

kem_papabear_ephem_cpa_upstream: $(OBJS_KEM_PAPABEAR_EPHEM_CPA_KEM)
	bash scripts/collect_objects.sh kem_papabear_ephem_cpa $(OBJS_KEM_PAPABEAR_EPHEM_CPA_KEM)
	bash scripts/symbols_global_rename.sh kem_papabear_ephem_cpa src/kem/threebears/symbols_global_rename_papabear_ephem_cpa.txt
	bash scripts/symbols_local.sh kem_papabear_ephem_cpa src/kem/threebears/symbols_local.txt
