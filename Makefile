CC=cc
AR=ar rcs
CURL=curl
RANLIB=ranlib
LN=ln -s

DEFAULTS=-DOQS_RAND_DEFAULT_URANDOM_CHACHA20 -DOQS_KEX_DEFAULT_BCNS15
CFLAGS=$(DEFAULTS) -DCONSTANT_TIME
INCLUDES=-Iinclude

all: links objdir rand_urandom_chacha20 rand kex_rlwe_bcns15 kex lib tests

objdir:
	mkdir -p objs
	rm -f liboqs.a

links:
	rm -rf include/oqs
	mkdir -p include/oqs
	$(LN) ../../src/kex/kex.h include/oqs
	$(LN) ../../src/kex_rlwe_bcns15/kex_rlwe_bcns15.h include/oqs
	$(LN) ../../src/rand/rand.h include/oqs
	$(LN) ../../src/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs

rand_urandom_chacha20: src/rand_urandom_chacha20/rand_urandom_chacha20.h src/rand_urandom_chacha20/rand_urandom_chacha20.c src/rand_urandom_chacha20/chacha20.c
	$(CC) -c $(CFLAGS) $(INCLUDES) src/rand_urandom_chacha20/rand_urandom_chacha20.c -o objs/rand_urandom_chacha20.o
	$(AR) liboqs.a objs/rand_urandom_chacha20.o

rand: src/rand/rand.h src/rand/rand.c
	$(CC) -c $(CFLAGS) $(INCLUDES) src/rand/rand.c -o objs/rand.o
	$(AR) liboqs.a objs/rand.o

kex_rlwe_bcns15: src/kex_rlwe_bcns15/fft.c src/kex_rlwe_bcns15/kex_rlwe_bcns15.c src/kex_rlwe_bcns15/kex_rlwe_bcns15.h src/kex_rlwe_bcns15/local.h src/kex_rlwe_bcns15/rlwe.c src/kex_rlwe_bcns15/rlwe_a.h src/kex_rlwe_bcns15/rlwe_kex.c src/kex_rlwe_bcns15/rlwe_table.h
	$(CC) -c $(CFLAGS) $(INCLUDES) src/kex_rlwe_bcns15/fft.c -o objs/kex_rlwe_bcns15_fft.o
	$(CC) -c $(CFLAGS) $(INCLUDES) src/kex_rlwe_bcns15/rlwe.c -o objs/kex_rlwe_bcns15_rlwe.o
	$(CC) -c $(CFLAGS) $(INCLUDES) src/kex_rlwe_bcns15/rlwe_kex.c -o objs/kex_rlwe_bcns15_rlwe_kex.o
	$(CC) -c $(CFLAGS) $(INCLUDES) src/kex_rlwe_bcns15/kex_rlwe_bcns15.c -o objs/kex_rlwe_bcns15.o
	$(AR) liboqs.a objs/kex_rlwe_bcns15_fft.o
	$(AR) liboqs.a objs/kex_rlwe_bcns15_rlwe.o
	$(AR) liboqs.a objs/kex_rlwe_bcns15_rlwe_kex.o
	$(AR) liboqs.a objs/kex_rlwe_bcns15.o

kex: src/kex/kex.h src/kex/kex.c
	$(CC) -c $(CFLAGS) $(INCLUDES) src/kex/kex.c -o objs/kex.o
	$(AR) liboqs.a objs/kex.o

lib:
	$(RANLIB) liboqs.a

tests: lib src/rand/test_rand.c src/kex/test_kex.c
	$(CC) $(CFLAGS) $(INCLUDES) -L. src/rand/test_rand.c -loqs -o test_rand 
	$(CC) $(CFLAGS) $(INCLUDES) -L. src/kex/test_kex.c -loqs -o test_kex

docs: links
	doxygen

clean:
	rm -rf docs
	rm -rf objs
	rm -rf include
	rm -f test_rand
	rm -f test_kex
	rm -f liboqs.a
	rm -f .DS_Store */.DS_Store */*/.DS_Store */*/*/.DS_Store

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none src/*/*.h src/*/*.c
