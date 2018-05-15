# THESE SHOULD BE THE ONLY OPTIONS TO BE CONFIGURED BY THE PERSON COMPILING

KEMS_TO_ENABLE=frodokem_640_aes frodokem_640_cshake frodokem_976_aes frodokem_976_cshake \
			   newhope_512_cca_kem newhope_1024_cca_kem \
			   kyber512 kyber768 kyber1024 \
			   bike1_l1 bike1_l3 bike1_l5 \
			   bike2_l1 bike2_l3 bike2_l5 \
			   bike3_l1 bike3_l3 bike3_l5 \
			   sike_p503, sike_p751 \
			   BIG_QUAKE_1 BIG_QUAKE_3 BIG_QUAKE_5 \
			   ledakem_C1_N02 ledakem_C1_N03 ledakem_C1_N04 \
			   ledakem_C3_N02 ledakem_C3_N03 ledakem_C3_N04 \
			   ledakem_C5_N02 ledakem_C5_N03 ledakem_C5_N04 # EDIT-WHEN-ADDING-KEM
KEM_DEFAULT=newhope_1024_cca_kem

ARCH=x64
# x64 OR x86

PREFIX=usr_local
PREFIX_INCLUDE=$(PREFIX)/include
PREFIX_LIB=$(PREFIX)/lib

CC=gcc
OPENSSL_INCLUDE_DIR=/usr/local/opt/openssl/include
OPENSSL_LIB_DIR=/usr/local/opt/openssl/lib
CFLAGS+= -fPIC
LDFLAGS=
CLANGFORMAT=clang-format




# NOTHING AFTER THIS SHOULD NEED TO BE CHANGED BY THE PERSON COMPILING

ENABLE_KEMS= # THIS WILL BE FILLED IN BY INDIVIDUAL KEMS' MAKEFILES IN COMBINATION WITH THE ARCHITECTURE

CFLAGS+=-g -std=c99 -Iinclude -I$(OPENSSL_INCLUDE_DIR) -Wno-unused-function -Werror -Wpedantic -Wall -Wextra
LDFLAGS+=-L$(OPENSSL_LIB_DIR) -lcrypto -lm

all: mkdirs headers liboqs tests speeds kats examples

OBJECT_DIRS=
TO_CLEAN=liboqs.a

include src/common/Makefile
include src/kem/Makefile

HEADERS=src/oqs.h $(HEADERS_COMMON) $(HEADERS_KEM)
OBJECTS=$(OBJECTS_COMMON) $(OBJECTS_KEM)

mkdirs:
	mkdir -p $(OBJECT_DIRS)

DATE=`date`
UNAME=`uname -a`
CC_VERSION=`$(CC) --version | tr '\n' ' '`
config_h:
	$(RM) -r src/config.h
	touch src/config.h
	echo "/**" >> src/config.h
	echo " * @file config.h" >> src/config.h
	echo " * @brief Pre-processor macros indicating compile-time options." >> src/config.h
	echo " */" >> src/config.h
	$(foreach ENABLE_KEM, $(ENABLE_KEMS), echo "/** Preprocessor macro indicating KEM $(ENABLE_KEM) is enabled. */" >> src/config.h; echo "#define OQS_ENABLE_KEM_$(ENABLE_KEM)" >> src/config.h;)
	echo "/** Preprocessor macro setting the default KEM to $(KEM_DEFAULT). */" >> src/config.h
	echo "#define OQS_KEM_DEFAULT OQS_KEM_alg_$(KEM_DEFAULT)" >> src/config.h
	echo "/** Date on which liboqs was compiled. */" >> src/config.h
	echo "#define OQS_COMPILE_DATE \"$(DATE)\"" >> src/config.h
	echo "/** Compiler command used to compile liboqs. */" >> src/config.h
	echo "#define OQS_COMPILE_CC \"$(CC)\"" >> src/config.h
	echo "/** Compiler version used to compile liboqs. */" >> src/config.h
	echo "#define OQS_COMPILE_CC_VERSION \"$(CC_VERSION)\"" >> src/config.h
	echo "/** CFLAGS version used to compile liboqs. */" >> src/config.h
	echo "#define OQS_COMPILE_CFLAGS \"$(CFLAGS)\"" >> src/config.h
	echo "/** LDFLAGS version used to compile liboqs. */" >> src/config.h
	echo "#define OQS_COMPILE_LDFLAGS \"$(LDFLAGS)\"" >> src/config.h
	echo "/** List of KEMs enabled at compile time. */" >> src/config.h
	echo "#define OQS_COMPILE_ENABLE_KEMS \"$(ENABLE_KEMS)\"" >> src/config.h
	echo "/** Which KEM is mapped to the default (OQS_KEM_alg_default). */" >> src/config.h
	echo "#define OQS_COMPILE_KEM_DEFAULT \"$(KEM_DEFAULT)\"" >> src/config.h
	echo "/** Platform on which liboqs was compiled. */" >> src/config.h
	echo "#define OQS_COMPILE_UNAME \"$(UNAME)\"" >> src/config.h


headers: config_h $(HEADERS)
	$(RM) -r include
	mkdir -p include/oqs
	cp $(HEADERS) src/config.h include/oqs

liboqs: mkdirs headers $(OBJECTS) $(UPSTREAMS)
	$(RM) -f liboqs.a
	ar rcs liboqs.a `find .objs -name '*.a'` `find .objs -name '*.o'`
	gcc -shared -o liboqs.so liboqs.a

TEST_PROGRAMS=test_kem
tests: $(TEST_PROGRAMS)

KAT_PROGRAMS=kat_kem
kats: $(KAT_PROGRAMS)

test: tests
	./test_kem

kat: kats
	./kat_kem
	scripts/check_kats.sh

SPEED_PROGRAMS=speed_kem
speeds: $(SPEED_PROGRAMS)

speed: speeds
	./speed_kem --info

EXAMPLE_PROGRAMS=example_kem
examples: $(EXAMPLE_PROGRAMS)

docs: headers
	mkdir -p docs/doxygen
	doxygen docs/.Doxyfile

install:
	@if [[ $(PREFIX) == "usr_local" ]] ; then echo "Installing to `pwd`/$(PREFIX). Override by running 'make install PREFIX=<destination>'."; fi
	mkdir -p $(PREFIX_INCLUDE)
	mkdir -p $(PREFIX_LIB)
	$(RM) -r $(PREFIX_INCLUDE)/oqs
	$(RM) $(PREFIX_LIB)/liboqs.a
	$(RM) $(PREFIX_LIB)/liboqs.so
	cp -r include/oqs $(PREFIX_INCLUDE)
	cp liboqs.a $(PREFIX_LIB)
	cp liboqs.so $(PREFIX_LIB)

clean:
	$(RM) -r includes
	$(RM) -r .objs
	$(RM) -r *.dSYM
	$(RM) -r kat_kem_rsp
	$(RM) liboqs.a liboqs.so
	$(RM) $(TO_CLEAN)
	$(RM) $(TEST_PROGRAMS)
	$(RM) $(KAT_PROGRAMS)
	$(RM) $(SPEED_PROGRAMS)
	$(RM) $(EXAMPLE_PROGRAMS)
	$(RM) -r docs/doxygen

check_namespacing: all
	.travis/global-namespace-check.sh

prettyprint:
	find src -name '*.c' -o -name '*.h' | grep -v upstream | xargs $(CLANGFORMAT) -style=file -i

pre-push:
	ARCH=x64 CC_OVERRIDE=gcc .travis/all-tests.sh
