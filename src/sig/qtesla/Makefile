ENABLE_SIGS+=$(findstring qTESLA_I, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring qTESLA_III_size, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring qTESLA_III_speed, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring qTESLA_p_I, $(SIGS_TO_ENABLE))
ENABLE_SIGS+=$(findstring qTESLA_p_III, $(SIGS_TO_ENABLE))

HEADERS_SIG_QTESLA=src/sig/qtesla/sig_qtesla.h
HEADERS_SIG+=$(HEADERS_SIG_QTESLA)

OBJECT_DIRS+=.objs/sig/qtesla
OBJECTS_SIG_QTESLA=.objs/sig/qtesla/sig_qtesla.o
OBJECTS_SIG+=$(OBJECTS_SIG_QTESLA)

.objs/sig/qtesla/sig_qtesla.o: headers src/sig/qtesla/sig_qtesla.c
	$(CC) -c src/sig/qtesla/sig_qtesla.c -o .objs/sig/qtesla/sig_qtesla.o $(CFLAGS)

ifneq (,$(findstring qTESLA_I, $(ENABLE_SIGS)))
UPSTREAMS+=sig_qTESLA_I_upstream
endif

SRCS_SIG_QTESLA_I=src/sig/qtesla/upstream/Reference_implementation/qTesla_I/sign.c src/sig/qtesla/upstream/Reference_implementation/qTesla_I/sample.c src/sig/qtesla/upstream/Reference_implementation/qTesla_I/poly.c src/sig/qtesla/upstream/Reference_implementation/qTesla_I/consts.c src/sig/qtesla/upstream/Reference_implementation/qTesla_I/sha3/fips202.c
OBJS_SIG_QTESLA_I=$(SRCS_SIG_QTESLA_I:.c=.o)

TO_CLEAN+= $(OBJS_SIG_QTESLA_I)

src/sig/qtesla/upstream/Reference_implementation/qTesla_I/%.o: src/sig/qtesla/upstream/Reference_implementation/qTesla_I/%.c
	$(CC) -c -fPIC -O3 -std=c99 -march=native -o $@ $<

sig_qTESLA_I_upstream: $(OBJS_SIG_QTESLA_I)
	bash scripts/collect_objects.sh sig_qTESLA_I $(OBJS_SIG_QTESLA_I)
	bash scripts/symbols_global_rename.sh sig_qTESLA_I src/sig/qtesla/symbols_global_rename_I.txt
	bash scripts/symbols_local.sh sig_qTESLA_I src/sig/qtesla/symbols_local.txt

ifneq (,$(findstring qTESLA_III_size, $(ENABLE_SIGS)))
UPSTREAMS+=sig_qTESLA_III_size_upstream
endif

SRCS_SIG_QTESLA_III_SIZE=src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/sign.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/sample.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/poly.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/consts.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/sha3/fips202.c
OBJS_SIG_QTESLA_III_SIZE=$(SRCS_SIG_QTESLA_III_SIZE:.c=.o)

TO_CLEAN+= $(OBJS_SIG_QTESLA_III_SIZE)

src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/%.o: src/sig/qtesla/upstream/Reference_implementation/qTesla_III_size/%.c
	$(CC) -c -fPIC -O3 -std=c99 -march=native -o $@ $<

sig_qTESLA_III_size_upstream: $(OBJS_SIG_QTESLA_III_SIZE)
	bash scripts/collect_objects.sh sig_qTESLA_III_size $(OBJS_SIG_QTESLA_III_SIZE)
	bash scripts/symbols_global_rename.sh sig_qTESLA_III_size src/sig/qtesla/symbols_global_rename_III_size.txt
	bash scripts/symbols_local.sh sig_qTESLA_III_size src/sig/qtesla/symbols_local.txt

ifneq (,$(findstring qTESLA_III_speed, $(ENABLE_SIGS)))
UPSTREAMS+=sig_qTESLA_III_speed_upstream
endif

SRCS_SIG_QTESLA_III_SPEED=src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/sign.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/sample.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/poly.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/consts.c src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/sha3/fips202.c
OBJS_SIG_QTESLA_III_SPEED=$(SRCS_SIG_QTESLA_III_SPEED:.c=.o)

TO_CLEAN+= $(OBJS_SIG_QTESLA_III_SPEED)

src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/%.o: src/sig/qtesla/upstream/Reference_implementation/qTesla_III_speed/%.c
	$(CC) -c -fPIC -O3 -std=c99 -march=native -o $@ $<

sig_qTESLA_III_speed_upstream: $(OBJS_SIG_QTESLA_III_SPEED)
	bash scripts/collect_objects.sh sig_qTESLA_III_speed $(OBJS_SIG_QTESLA_III_SPEED)
	bash scripts/symbols_global_rename.sh sig_qTESLA_III_speed src/sig/qtesla/symbols_global_rename_III_speed.txt
	bash scripts/symbols_local.sh sig_qTESLA_III_speed src/sig/qtesla/symbols_local.txt

ifneq (,$(findstring qTESLA_p_I, $(ENABLE_SIGS)))
UPSTREAMS+=sig_qTESLA_p_I_upstream
endif

SRCS_SIG_QTESLA_P_I=src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/sign.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/sample.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/poly.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/consts.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/sha3/fips202.c
OBJS_SIG_QTESLA_P_I=$(SRCS_SIG_QTESLA_P_I:.c=.o)

TO_CLEAN+= $(OBJS_SIG_QTESLA_P_I)

src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/%.o: src/sig/qtesla/upstream/Reference_implementation/qTesla_p_I/%.c
	$(CC) -c -fPIC -O3 -std=c99 -march=native -o $@ $<

sig_qTESLA_p_I_upstream: $(OBJS_SIG_QTESLA_P_I)
	bash scripts/collect_objects.sh sig_qTESLA_p_I $(OBJS_SIG_QTESLA_P_I)
	bash scripts/symbols_global_rename.sh sig_qTESLA_p_I src/sig/qtesla/symbols_global_rename_p_I.txt
	bash scripts/symbols_local.sh sig_qTESLA_p_I src/sig/qtesla/symbols_local.txt

ifneq (,$(findstring qTESLA_p_III, $(ENABLE_SIGS)))
UPSTREAMS+=sig_qTESLA_p_III_upstream
endif

SRCS_SIG_QTESLA_P_III=src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/sign.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/sample.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/poly.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/consts.c src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/sha3/fips202.c
OBJS_SIG_QTESLA_P_III=$(SRCS_SIG_QTESLA_P_III:.c=.o)

TO_CLEAN+= $(OBJS_SIG_QTESLA_P_III)

src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/%.o: src/sig/qtesla/upstream/Reference_implementation/qTesla_p_III/%.c
	$(CC) -c -fPIC -O3 -std=c99 -march=native -o $@ $<

sig_qTESLA_p_III_upstream: $(OBJS_SIG_QTESLA_P_III)
	bash scripts/collect_objects.sh sig_qTESLA_p_III $(OBJS_SIG_QTESLA_P_III)
	bash scripts/symbols_global_rename.sh sig_qTESLA_p_III src/sig/qtesla/symbols_global_rename_p_III.txt
	bash scripts/symbols_local.sh sig_qTESLA_p_III src/sig/qtesla/symbols_local.txt

