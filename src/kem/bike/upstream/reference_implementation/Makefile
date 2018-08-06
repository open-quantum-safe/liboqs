TARGET = bike$(VER)_$(LEVEL).o

CSRCS =  aes_ctr_prf.c kem.c utilities.c conversions.c decode.c parallel_hash.c sampling.c
OBJS=$(CSRCS:.c=.o)

#WA for GCC 4.8.5 bug.
EXTRA_FLAGS += -Wno-missing-braces -Wno-missing-field-initializers


ifeq ($(VER),1)
    EXTRA_FLAGS += -DBIKE1=1
endif
ifeq ($(VER),2)
    EXTRA_FLAGS += -DBIKE2=1
endif
ifeq ($(VER),3)
    EXTRA_FLAGS += -DBIKE3=1
endif

ifeq ($(LEVEL),1)
    EXTRA_FLAGS += -DPARAM64=1
endif
ifeq ($(LEVEL),3)
    EXTRA_FLAGS += -DPARAM96=1
endif
ifeq ($(LEVEL),5)
    EXTRA_FLAGS += -DPARAM128=1
endif

%.o: %.c
	$(CC) -c $(CFLAGS) $(EXTRA_FLAGS) -I"$(INC_DIR)" -o  $@ $< -I$(OPENSSL_INCLUDE_DIR)

all: $(OBJS)

clean: 
	rm -rf *.o
