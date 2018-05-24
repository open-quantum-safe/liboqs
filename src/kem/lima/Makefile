ifeq (x64,$(ARCH))
ENABLE_KEMS+=$(findstring lima_2p_1024_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_2p_2048_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1018_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1306_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1822_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_2062_cca_kem, $(KEMS_TO_ENABLE))
else ifeq (x86,$(ARCH))
ENABLE_KEMS+=$(findstring lima_2p_1024_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_2p_2048_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1018_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1306_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_1822_cca_kem, $(KEMS_TO_ENABLE))
ENABLE_KEMS+=$(findstring lima_sp_2062_cca_kem, $(KEMS_TO_ENABLE))
endif

HEADERS_KEM_LIMA=src/kem/lima/kem_lima.h
HEADERS_KEM+=$(HEADERS_KEM_LIMA)

OBJECT_DIRS+=.objs/kem/lima
OBJECTS_KEM_LIMA=.objs/kem/lima/kem_lima.o
OBJECTS_KEM+=$(OBJECTS_KEM_LIMA)

.objs/kem/lima/kem_lima.o: headers src/kem/lima/kem_lima.c
	$(CC) -c src/kem/lima/kem_lima.c -o .objs/kem/lima/kem_lima.o $(CFLAGS)

ifneq (,$(findstring lima_2p_1024_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_2p_1024_cca_kem_upstream
endif
ifneq (,$(findstring lima_2p_2048_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_2p_2048_cca_kem_upstream
endif
ifneq (,$(findstring lima_sp_1018_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_sp_1018_cca_kem_upstream
endif
ifneq (,$(findstring lima_sp_1306_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_sp_1306_cca_kem_upstream
endif
ifneq (,$(findstring lima_sp_1822_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_sp_1822_cca_kem_upstream
endif
ifneq (,$(findstring lima_sp_2062_cca_kem, $(ENABLE_KEMS)))
UPSTREAMS+=kem_lima_sp_2062_cca_kem_upstream
endif

SRCS_KEM_LIMA_2P_1024_CCA_KEM= src/kem/lima/upstream/optimized-implementation-master/lib/fft.c src/kem/lima/upstream/optimized-implementation-master/lib/lima.c src/kem/lima/upstream/optimized-implementation-master/lib/modp.c src/kem/lima/upstream/optimized-implementation-master/lib/xof.c src/kem/lima/upstream/optimized-implementation-master/lib/LimaAPI.c
OBJS_KEM_LIMA_2P_1024_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM:.c=.o)

SRCS_KEM_LIMA_2P_2048_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM)
OBJS_KEM_LIMA_2P_2048_CCA_KEM=$(SRCS_KEM_LIMA_2P_2048_CCA_KEM:.c=.o)

SRCS_KEM_LIMA_SP_1018_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM)
OBJS_KEM_LIMA_SP_1018_CCA_KEM=$(SRCS_KEM_LIMA_SP_1018_CCA_KEM:.c=.o)

SRCS_KEM_LIMA_SP_1306_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM)
OBJS_KEM_LIMA_SP_1306_CCA_KEM=$(SRCS_KEM_LIMA_SP_1306_CCA_KEM:.c=.o)

SRCS_KEM_LIMA_SP_1822_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM)
OBJS_KEM_LIMA_SP_1822_CCA_KEM=$(SRCS_KEM_LIMA_SP_1822_CCA_KEM:.c=.o)

SRCS_KEM_LIMA_SP_2062_CCA_KEM=$(SRCS_KEM_LIMA_2P_1024_CCA_KEM)
OBJS_KEM_LIMA_SP_2062_CCA_KEM=$(SRCS_KEM_LIMA_SP_2062_CCA_KEM:.c=.o)

TO_CLEAN+= $(OBJS_KEM_LIMA_2P_1024_CCA_KEM)
TO_CLEAN+= $(OBJS_KEM_LIMA_2P_2048_CCA_KEM)
TO_CLEAN+= $(OBJS_KEM_LIMA_SP_1018_CCA_KEM)
TO_CLEAN+= $(OBJS_KEM_LIMA_SP_1306_CCA_KEM)
TO_CLEAN+= $(OBJS_KEM_LIMA_SP_1822_CCA_KEM)
TO_CLEAN+= $(OBJS_KEM_LIMA_SP_2062_CCA_KEM)

src/kem/lima/upstream/optimized-implementation-master/lib/%.o: src/kem/lima/upstream/optimized-implementation-master/lib/%.c libkeccak
	$(CC) -c -fPIC -O3 -std=c99 -o $@ $< -I$(OPENSSL_INCLUDE_DIR) -I$(KECCAK_INCLUDE_DIR)

kem_lima_2p_1024_cca_kem_upstream: $(OBJS_KEM_LIMA_2P_1024_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=1024 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_2p_1024_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_2p_1024_cca_kem $(OBJS_KEM_LIMA_2P_1024_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_2p_1024_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_2p_1024_cca_kem src/kem/lima/symbols_global_rename_2p_1024.txt
	bash scripts/symbols_local.sh kem_lima_2p_1024_cca_kem src/kem/lima/symbols_local.txt

kem_lima_2p_2048_cca_kem_upstream: $(OBJS_KEM_LIMA_2P_2048_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=2048 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_2p_2048_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_2p_2048_cca_kem $(OBJS_KEM_LIMA_2P_2048_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_2p_2048_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_2p_2048_cca_kem src/kem/lima/symbols_global_rename_2p_2048.txt
	bash scripts/symbols_local.sh kem_lima_2p_2048_cca_kem src/kem/lima/symbols_local.txt

kem_lima_sp_1018_cca_kem_upstream: $(OBJS_KEM_LIMA_SP_1018_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=1018 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1018_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_sp_1018_cca_kem $(OBJS_KEM_LIMA_SP_1018_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1018_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_sp_1018_cca_kem src/kem/lima/symbols_global_rename_sp_1018.txt
	bash scripts/symbols_local.sh kem_lima_sp_1018_cca_kem src/kem/lima/symbols_local.txt

kem_lima_sp_1306_cca_kem_upstream: $(OBJS_KEM_LIMA_SP_1306_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=1306 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1306_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_sp_1306_cca_kem $(OBJS_KEM_LIMA_SP_1306_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1306_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_sp_1306_cca_kem src/kem/lima/symbols_global_rename_sp_1306.txt
	bash scripts/symbols_local.sh kem_lima_sp_1306_cca_kem src/kem/lima/symbols_local.txt

kem_lima_sp_1822_cca_kem_upstream: $(OBJS_KEM_LIMA_SP_1822_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=1822 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1822_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_sp_1822_cca_kem $(OBJS_KEM_LIMA_SP_1822_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_1822_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_sp_1822_cca_kem src/kem/lima/symbols_global_rename_sp_1822.txt
	bash scripts/symbols_local.sh kem_lima_sp_1822_cca_kem src/kem/lima/symbols_local.txt

kem_lima_sp_2062_cca_kem_upstream: $(OBJS_KEM_LIMA_SP_2062_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c
	$(CC) -c -fPIC -O3 -std=c99 -DLIMASZ=2062 -o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_2062_cca.o src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem.c -Isrc/kem/lima/upstream/optimized-implementation-master/Lib -I$(KECCAK_INCLUDE_DIR)
	bash scripts/collect_objects.sh kem_lima_sp_2062_cca_kem $(OBJS_KEM_LIMA_SP_2062_CCA_KEM) src/kem/lima/upstream/optimized-implementation-master/NIST/EncapCCA/kem_lima_sp_2062_cca.o
	bash scripts/symbols_global_rename.sh kem_lima_sp_2062_cca_kem src/kem/lima/symbols_global_rename_sp_2062.txt
	bash scripts/symbols_local.sh kem_lima_sp_2062_cca_kem src/kem/lima/symbols_local.txt
