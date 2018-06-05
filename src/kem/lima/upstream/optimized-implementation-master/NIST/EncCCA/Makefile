CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -L./../../Lib -I./../../Lib -O3 -march=native
LIBS = -lLima -lkeccak -lcrypto -lm

all: KATFiles

PQCgenKAT_encrypt-2048.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=2048 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-2048.exe $(LIBS)

PQCgenKAT_encrypt-1018.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=1018 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-1018.exe $(LIBS)

PQCgenKAT_encrypt-1306.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=1306 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-1306.exe $(LIBS)

PQCgenKAT_encrypt-1822.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=1822 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-1822.exe $(LIBS)

PQCgenKAT_encrypt-2062.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=2062 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-2062.exe $(LIBS)

PQCgenKAT_encrypt-1024.exe:  encrypt.c api.h rng.c rng.h PQCgenKAT_encrypt.c
	$(CC) $(CFLAGS) -DLIMASZ=1024 PQCgenKAT_encrypt.c encrypt.c rng.c -o PQCgenKAT_encrypt-1024.exe $(LIBS)

KATFiles: PQCgenKAT_encrypt-2048.exe PQCgenKAT_encrypt-1018.exe PQCgenKAT_encrypt-1306.exe PQCgenKAT_encrypt-1822.exe PQCgenKAT_encrypt-2062.exe PQCgenKAT_encrypt-1024.exe
	./PQCgenKAT_encrypt-2048.exe 
	./PQCgenKAT_encrypt-1018.exe 
	./PQCgenKAT_encrypt-1306.exe 
	./PQCgenKAT_encrypt-1822.exe 
	./PQCgenKAT_encrypt-2062.exe 
	./PQCgenKAT_encrypt-1024.exe

clean:
	rm *.exe *.rsp *.req
