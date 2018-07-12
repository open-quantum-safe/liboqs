CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -L./../../Lib -I./../../Lib -O3 -march=native
LIBS = -lLima -lkeccak -lcrypto -lm

all: KATFiles

PQCgenKAT_kem-2048.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=2048 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-2048.exe $(LIBS)

PQCgenKAT_kem-1018.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=1018 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-1018.exe $(LIBS)


PQCgenKAT_kem-1306.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=1306 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-1306.exe $(LIBS)

PQCgenKAT_kem-1822.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=1822 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-1822.exe $(LIBS)

PQCgenKAT_kem-2062.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=2062 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-2062.exe $(LIBS)

PQCgenKAT_kem-1024.exe:  kem.c api.h rng.c rng.h PQCgenKAT_kem.c
	$(CC) $(CFLAGS) -DLIMASZ=1024 PQCgenKAT_kem.c kem.c rng.c -o PQCgenKAT_kem-1024.exe $(LIBS)

KATFiles: PQCgenKAT_kem-2048.exe PQCgenKAT_kem-1018.exe PQCgenKAT_kem-1306.exe PQCgenKAT_kem-1822.exe PQCgenKAT_kem-2062.exe PQCgenKAT_kem-1024.exe
	./PQCgenKAT_kem-2048.exe 
	./PQCgenKAT_kem-1018.exe 
	./PQCgenKAT_kem-1306.exe 
	./PQCgenKAT_kem-1822.exe 
	./PQCgenKAT_kem-2062.exe 
	./PQCgenKAT_kem-1024.exe

clean:
	rm *.exe *.rsp *.req
