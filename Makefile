CC=cc
AR=ar rcs
CURL=curl
RANLIB=ranlib
LN=ln -s

DEFAULTS= -std=gnu11 -Wpedantic -Wall -DOQS_RAND_DEFAULT_URANDOM_CHACHA20 -DOQS_KEX_DEFAULT_BCNS15
CFLAGS=$(DEFAULTS) -DCONSTANT_TIME
LDFLAGS=-lm
INCLUDES=-Iinclude

all: links lib tests

objs/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -c  $(CFLAGS) $(INCLUDES) $< -o $@

links:
	rm -rf include/oqs
	mkdir -p include/oqs
	$(LN) ../../src/kex/kex.h include/oqs
	$(LN) ../../src/kex_rlwe_bcns15/kex_rlwe_bcns15.h include/oqs
	$(LN) ../../src/rand/rand.h include/oqs
	$(LN) ../../src/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs

# CHACHA

CHACHA_OBJS :=  $(addprefix objs/rand_urandom_chacha20/, \
                rand_urandom_chacha20.o)

$(CHACHA_OBJS): src/rand_urandom_chacha20/rand_urandom_chacha20.h src/rand_urandom_chacha20/chacha20.c

# RAND

objs/rand/rand.o: src/rand/rand.h

# BCNS

BCNS_OBJS := $(addprefix objs/kex_rlwe_bcns15/, \
             fft.o kex_rlwe_bcns15.o rlwe.o rlwe_kex.o)

BCNS_HEADERS := $(addprefix src/kex_rlwe_bcns15/, \
                kex_rlwe_bcns15.h local.h rlwe_a.h rlwe_table.h)

$(BCNS_OBJS): $(BCNS_HEADERS)

# KEX

objs/kex/kex.o: src/kex/kex.h

#LIB

lib: $(CHACHA_OBJS) $(BCNS_OBJS) objs/rand/rand.o objs/kex/kex.o
	rm -f liboqs.a
	$(AR) liboqs.a $^
	$(RANLIB) liboqs.a

tests: lib src/rand/test_rand.c src/kex/test_kex.c
	$(CC) $(CFLAGS) $(INCLUDES) -L. src/rand/test_rand.c -loqs $(LDFLAGS) -o test_rand 
	$(CC) $(CFLAGS) $(INCLUDES) -L. src/kex/test_kex.c -loqs $(LDFLAGS) -o test_kex

docs: links
	doxygen

clean:
	rm -rf docs objs include
	rm -f test_rand test_kex liboqs.a
	find . -name .DS_Store -type f -delete

prettyprint:
	astyle --style=java --indent=tab --pad-header --pad-oper --align-pointer=name --align-reference=name --suffix=none src/*/*.h src/*/*.c
