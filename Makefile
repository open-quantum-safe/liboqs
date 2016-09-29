CC=cc
AR=ar rcs
CURL=curl
RANLIB=ranlib
LN=ln -s

# Defaults algorithms
DEFAULTS_ALGORITHMS= -DOQS_RAND_DEFAULT_URANDOM_CHACHA20 -DOQS_KEX_DEFAULT_BCNS15
# Defaults flags
DEFAULTS= -std=gnu11 -Wpedantic -Wall -Wextra

CFLAGS=$(DEFAULTS) $(DEFAULTS_ALGORITHMS) -DCONSTANT_TIME
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
	$(LN) ../../src/sign/sign.h include/oqs

# RAND_URANDOM_CHACHA

RAND_URANDOM_CHACHA_OBJS :=  $(addprefix objs/rand_urandom_chacha20/, rand_urandom_chacha20.o)

$(RAND_URANDOM_CHACHA_OBJS): src/rand_urandom_chacha20/rand_urandom_chacha20.h

# RAND

objs/rand/rand.o: src/rand/rand.h

# KEX_RLWE_BCNS15

KEX_RLWE_BCNS15_OBJS := $(addprefix objs/kex_rlwe_bcns15/, fft.o kex_rlwe_bcns15.o rlwe.o rlwe_kex.o)

KEX_RLWE_BCNS15_HEADERS := $(addprefix src/kex_rlwe_bcns15/, kex_rlwe_bcns15.h local.h rlwe_a.h rlwe_table.h)

$(KEX_RLWE_BCNS15_OBJS): $(KEX_RLWE_BCNS15_HEADERS)

# KEX

objs/kex/kex.o: src/kex/kex.h

# SIGN

objs/sign/sign.o: src/sign/sign.h

# LIB

lib: $(RAND_URANDOM_CHACHA_OBJS) $(KEX_RLWE_BCNS15_OBJS) objs/rand/rand.o objs/kex/kex.o # objs/sign/sign.o
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
