SEC     = 128
CC	= gcc
LDFLAGS = -lcrypto
CFLAGS	= -ansi -O3 -march=native -mtune=native -fomit-frame-pointer -funroll-loops -Wall -Wextra -D_SEC=$(SEC)
DIR     = lotus$(SEC)
KDIR	= ../../KAT/PQCgenKAT
INCL	= -I./$(DIR)
SRC	= $(wildcard $(DIR)/*.c) $(KDIR)/rng.c
OBJ	= $(SRC:.c=.o)

PQCgenKAT_kem: $(OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCL) -o $@ $(KDIR)/PQCgenKAT_kem.c $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCL) -o $@ -c $<

clean:
	rm $(OBJ) PQCgenKAT_kem
