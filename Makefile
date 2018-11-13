# THESE SHOULD BE THE ONLY OPTIONS TO BE CONFIGURED BY THE PERSON COMPILING

KEMS_TO_ENABLE?=frodokem_640_aes frodokem_640_cshake frodokem_976_aes frodokem_976_cshake \
			   newhope_512_cca_kem newhope_1024_cca_kem \
			   kyber512 kyber768 kyber1024 \
			   bike1_l1 bike1_l3 bike1_l5 \
			   bike2_l1 bike2_l3 bike2_l5 \
			   bike3_l1 bike3_l3 bike3_l5 \
			   sike_p503, sike_p751 \
			   BIG_QUAKE_1 BIG_QUAKE_3 BIG_QUAKE_5 \
			   ledakem_C1_N02 ledakem_C1_N03 ledakem_C1_N04 \
			   ledakem_C3_N02 ledakem_C3_N03 ledakem_C3_N04 \
			   ledakem_C5_N02 ledakem_C5_N03 ledakem_C5_N04 \
			   saber_light_saber_kem saber_saber_kem saber_fire_saber_kem \
			   lima_2p_1024_cca_kem lima_2p_2048_cca_kem lima_sp_1018_cca_kem lima_sp_1306_cca_kem lima_sp_1822_cca_kem lima_sp_2062_cca_kem \
			   titanium_cca_std_kem titanium_cca_hi_kem titanium_cca_med_kem titanium_cca_super_kem
			   # EDIT-WHEN-ADDING-KEM

KEM_DEFAULT?=newhope_1024_cca_kem

SIGS_TO_ENABLE?=qTESLA_I qTESLA_III_size qTESLA_III_speed qTESLA_p_I qTESLA_p_III \
                picnic_L1_FS picnic_L1_UR picnic_L3_FS picnic_L3_UR picnic_L5_FS picnic_L5_UR \
                Dilithium_II_medium Dilithium_III_recommended Dilithium_IV_very_high # EDIT-WHEN-ADDING-SIG

SIG_DEFAULT?=qTESLA_I

ARCH?=x64
# x64 OR x86

#Currently checking CPUID only on Linux machines this
#Should be extended to other system in the future.
DETECTED_OS = $(shell uname -s)
ifeq ($(DETECTED_OS), Linux)
  AVX_SUPPORT = $(shell grep avx /proc/cpuinfo)
  AVX2_SUPPORT = $(shell grep avx2 /proc/cpuinfo)
  AVX512_SUPPORT = $(shell grep avx512 /proc/cpuinfo)

  export AVX_SUPPORT
  export AVX2_SUPPORT
  export AVX512_SUPPORT
endif

PREFIX?=usr_local
PREFIX_INCLUDE?=$(PREFIX)/include
PREFIX_LIB?=$(PREFIX)/lib

CC?=gcc
OPENSSL_INCLUDE_DIR?=/usr/local/opt/openssl/include
OPENSSL_LIB_DIR?=/usr/local/opt/openssl/lib
CFLAGS+= -fPIC
LDFLAGS?=
CLANGFORMAT?=clang-format

# NOTHING AFTER THIS SHOULD NEED TO BE CHANGED BY THE PERSON COMPILING

ENABLE_KEMS= # THIS WILL BE FILLED IN BY INDIVIDUAL KEMS' MAKEFILES IN COMBINATION WITH THE ARCHITECTURE

ENABLE_SIGS= # THIS WILL BE FILLED IN BY INDIVIDUAL SIGS' MAKEFILES IN COMBINATION WITH THE ARCHITECTURE

CFLAGS+=-O2 -std=c99 -Iinclude -I$(OPENSSL_INCLUDE_DIR) -Wno-unused-function -Werror -Wpedantic -Wall -Wextra
ifeq ($(arch), "x64")
  CFLAGS+= -arch x86_64
endif

ifeq ($(DETECTED_OS), Linux)
  ifneq (,$(AVX512_SUPPORT))
    CFLAGS+=-DAVX512
  else ifneq (,$(AVX2_SUPPORT))
    CFLAGS+=-DAVX2
  else ifneq (,$(AVX_SUPPORT))
    CFLAGS+=-DAVX
  endif
endif

LDFLAGS+=-Wl,-rpath,${OPENSSL_LIB_DIR}
ifeq ($(DETECTED_OS), Linux)
LDFLAGS+=-Wl,--enable-new-dtags
endif
LDFLAGS+=-L$(OPENSSL_LIB_DIR) -lcrypto -lm

KECCAK_INCLUDE_DIR=vendor/XKCP-master/bin/generic64
KECCAK_LIB_DIR=vendor/XKCP-master/bin/generic64

all: liboqs tests speeds kats examples

OBJECT_DIRS=
TO_CLEAN=liboqs.a

include src/common/Makefile
include src/kem/Makefile
include src/sig/Makefile

HEADERS=src/oqs.h $(HEADERS_COMMON) $(HEADERS_KEM) $(HEADERS_SIG)
OBJECTS=$(OBJECTS_COMMON) $(OBJECTS_KEM) $(OBJECTS_SIG)

mkdirs:
	mkdir -p $(OBJECT_DIRS)
	mkdir -p .objs_upstream

DATE=`date`
UNAME=`uname -a`
CC_VERSION=`$(CC) --version | tr '\n' ' '`
config_h:
	$(RM) -r src/oqsconfig.h
	touch src/oqsconfig.h
	echo "/**" >> src/oqsconfig.h
	echo " * @file config.h" >> src/oqsconfig.h
	echo " * @brief Pre-processor macros indicating compile-time options." >> src/oqsconfig.h
	echo " */" >> src/oqsconfig.h
	# Numeric release version identifier:
	# MYYYYMMS: major year month status
	# The status nibble has one of the values 0 for development, 1 to e for release candidates 1 to 4, and f for release
	# For example:
	# 2018-11-dev      0x02018110
	# 2018-11-rc1      0x02018111
	# 2018-11-rc2-dev  0x02018112
	# 2018-11-rc2      0x02018112 (same as ...rc2-dev)
	# 2018-11          0x0201811f
	echo "#define OQS_VERSION_NUMBER 0x0201811fL" >> src/oqsconfig.h
	echo "#define OQS_VERSION_TEXT \"2018-11\"" >> src/oqsconfig.h
	echo "/** liboqs branch. */" >> src/oqsconfig.h
	echo "#define OQS_NIST_BRANCH" >> src/oqsconfig.h
	$(foreach ENABLE_KEM, $(ENABLE_KEMS), echo "/** Preprocessor macro indicating KEM $(ENABLE_KEM) is enabled. */" >> src/oqsconfig.h; echo "#define OQS_ENABLE_KEM_$(ENABLE_KEM)" >> src/oqsconfig.h;)
	$(foreach ENABLE_SIG, $(ENABLE_SIGS), echo "/** Preprocessor macro indicating SIG $(ENABLE_SIG) is enabled. */" >> src/oqsconfig.h; echo "#define OQS_ENABLE_SIG_$(ENABLE_SIG)" >> src/oqsconfig.h;)
	echo "/** Preprocessor macro setting the default KEM to $(KEM_DEFAULT). */" >> src/oqsconfig.h
	echo "#define OQS_KEM_DEFAULT OQS_KEM_alg_$(KEM_DEFAULT)" >> src/oqsconfig.h
	echo "/** Preprocessor macro setting the default SIG to $(SIG_DEFAULT). */" >> src/oqsconfig.h
	echo "#define OQS_SIG_DEFAULT OQS_SIG_alg_$(SIG_DEFAULT)" >> src/oqsconfig.h
	echo "/** Date on which liboqs was compiled. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_DATE \"$(DATE)\"" >> src/oqsconfig.h
	echo "/** Compiler command used to compile liboqs. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_CC \"$(CC)\"" >> src/oqsconfig.h
	echo "/** Compiler version used to compile liboqs. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_CC_VERSION \"$(CC_VERSION)\"" >> src/oqsconfig.h
	echo "/** CFLAGS version used to compile liboqs. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_CFLAGS \"$(CFLAGS)\"" >> src/oqsconfig.h
	echo "/** LDFLAGS version used to compile liboqs. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_LDFLAGS \"$(LDFLAGS)\"" >> src/oqsconfig.h
	echo "/** List of KEMs enabled at compile time. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_ENABLE_KEMS \"$(ENABLE_KEMS)\"" >> src/oqsconfig.h
	echo "/** List of SIGs enabled at compile time. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_ENABLE_SIGS \"$(ENABLE_SIGS)\"" >> src/oqsconfig.h
	echo "/** Which KEM is mapped to the default (OQS_KEM_alg_default). */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_KEM_DEFAULT \"$(KEM_DEFAULT)\"" >> src/oqsconfig.h
	echo "/** Which SIG is mapped to the default (OQS_SIG_alg_default). */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_SIG_DEFAULT \"$(SIG_DEFAULT)\"" >> src/oqsconfig.h
	echo "/** Platform on which liboqs was compiled. */" >> src/oqsconfig.h
	echo "#define OQS_COMPILE_UNAME \"$(UNAME)\"" >> src/oqsconfig.h

headers: config_h mkdirs
	$(RM) -r include
	mkdir -p include/oqs
	cp $(HEADERS) src/oqsconfig.h include/oqs

libkeccak:
	bash scripts/build-keccak-code-package.sh
	$(RM) -rf .objs/keccak
	mkdir -p .objs/keccak
	cd .objs/keccak && ar x ../../vendor/XKCP-master/bin/generic64/libkeccak.a

liboqs: libkeccak headers $(OBJECTS) $(UPSTREAMS)
	$(RM) -f liboqs.a
	ar rcs liboqs.a `find .objs -name '*.a'` `find .objs -name '*.o'`
	gcc -shared -o liboqs.so `find .objs -name '*.a'` `find .objs -name '*.o'` ${LDFLAGS}

TEST_PROGRAMS=test_kem test_kem_shared test_sig test_sig_shared
$(TEST_PROGRAMS): liboqs
tests: $(TEST_PROGRAMS)

KAT_PROGRAMS=kat_kem kat_sig
$(KAT_PROGRAMS): liboqs
kats: $(KAT_PROGRAMS)

test: tests
	./test_kem
	./test_sig

kat: kats
	./kat_kem
	./kat_sig
	scripts/check_kats.sh

SPEED_PROGRAMS=speed_kem speed_sig
$(SPEED_PROGRAMS): liboqs
speeds: $(SPEED_PROGRAMS)

speed: speeds
	./speed_kem --info
	./speed_sig --info

EXAMPLE_PROGRAMS=example_kem example_sig
$(EXAMPLE_PROGRAMS): liboqs
examples: $(EXAMPLE_PROGRAMS)

docs: headers
	mkdir -p docs/doxygen
	doxygen docs/.Doxyfile

install-noshared:
	@if [[ $(PREFIX) == "usr_local" ]] ; then echo "Installing to `pwd`/$(PREFIX). Override by running 'make install PREFIX=<destination>'."; fi
	mkdir -p $(PREFIX_INCLUDE)
	mkdir -p $(PREFIX_LIB)
	$(RM) -r $(PREFIX_INCLUDE)/oqs
	$(RM) $(PREFIX_LIB)/liboqs.a
	$(RM) $(PREFIX_LIB)/liboqs.so
	cp -r include/oqs $(PREFIX_INCLUDE)
	cp liboqs.a $(PREFIX_LIB)

install: install-noshared
	cp liboqs.so $(PREFIX_LIB)

clean:
	$(RM) -r includes
	$(RM) -r .objs
	$(RM) -r *.dSYM
	$(RM) -r kat_kem_rsp kat_sig_rsp
	$(RM) -r .objs_upstream
	$(RM) liboqs.a liboqs.so
	$(RM) $(TO_CLEAN)
	$(RM) $(TEST_PROGRAMS)
	$(RM) $(KAT_PROGRAMS)
	$(RM) $(SPEED_PROGRAMS)
	$(RM) $(EXAMPLE_PROGRAMS)
	$(RM) -r docs/doxygen
	$(RM) -r vendor/XKCP-master

check_namespacing: all
	.travis/global-namespace-check.sh

prettyprint:
	find src -name '*.c' -o -name '*.h' | grep -v upstream | xargs $(CLANGFORMAT) -style=file -i

pre-push:
	ARCH=x64 CC_OVERRIDE=gcc .travis/all-tests.sh
