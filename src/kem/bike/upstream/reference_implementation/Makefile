TARGET = bike$(VER)_$(LEVEL).o

CSRCS =  aes_ctr_prf.c kem.c utilities.c conversions.c decode.c 
CSRCS += parallel_hash.c sampling.c openssl_utils.c 
OBJS=$(CSRCS:.c=.o)

#WA for GCC 4.8.5 bug.
EXTRA_FLAGS += -Wno-missing-braces -Wno-missing-field-initializers
EXTRA_FLAGS += -DBIKE_VER=$(VER) -DLEVEL=$(LEVEL)

%.o: %.c
	$(CC) -c $(CFLAGS) $(EXTRA_FLAGS) -I"$(INC_DIR)" -o  $@ $< -I$(OPENSSL_INCLUDE_DIR)

all: $(OBJS)

clean: 
	rm -rf *.o
