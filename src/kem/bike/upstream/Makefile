TARGET = bike$(VER)_$(LEVEL).o

CSRCS =  aes_ctr_prf.c kem.c utilities.c conversions.c decode.c parallel_hash.c sampling.c
OBJS=$(CSRCS:.c=.o)

ifeq ($(VER),1)
    FLAGS += -DBIKE1=1
endif
ifeq ($(VER),2)
    FLAGS += -DBIKE2=1
endif
ifeq ($(VER),3)
    FLAGS += -DBIKE3=1
endif

ifeq ($(LEVEL),1)
    FLAGS += -DPARAM64=1
endif
ifeq ($(LEVEL),3)
    FLAGS += -DPARAM96=1
endif
ifeq ($(LEVEL),5)
    FLAGS += -DPARAM128=1
endif

%.o: %.c
	$(CC) -c $(FLAGS) -std=c99 -fPIC -O3 -I$(INC_DIR) -o  $@ $< -I$(OPENSSL_INCLUDE_DIR)

all: $(OBJS)

clean: 
	rm -rf *.o
