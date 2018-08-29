TARGET = bike$(VER)_$(LEVEL).o

include inc.mk

CSRCS =  aes.c decode.c kem.c sampling.c sha-mb.c aes_ctr_prf.c gf2x_mul.c parallel_hash.c 
CSRCS += sha.c utilities.c openssl_utils.c

SSRCS = converts.S gf_add.S sha384_update_mb.S vaes256_key_expansion.S red.S gf_mul.S
SSRCS += sampling${SUF}.S
ifdef CONSTANT_TIME
    SSRCS += secure_decode${SUF}.S
else
    SSRCS += decode${SUF}.S 
endif

OBJS=$(CSRCS:.c=.o)
OBJS+=$(SSRCS:.S=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $(EXTRA_FLAGS) -I$(INC_DIR) -o  $@ $< -I$(OPENSSL_INCLUDE_DIR)

%.o: %.S
	$(CC) -c $(CFLAGS) $(EXTRA_FLAGS) -I$(INC_DIR) -o  $@ $< -I$(OPENSSL_INCLUDE_DIR)

all: $(OBJS)

clean: 
	rm -rf *.o