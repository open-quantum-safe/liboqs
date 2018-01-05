# THESE SHOULD BE THE ONLY OPTIONS TO BE CONFIGURED BY THE PERSON COMPILING

ENABLE_KEMS=dummy1 dummy2
KEM_DEFAULT=dummy1

CC=gcc
OPENSSL_INCLUDE_DIR=/usr/local/opt/openssl/include
OPENSSL_LIB_DIR=/usr/local/opt/openssl/lib
CFLAGS=
LDFLAGS=

# NOTHING AFTER THIS SHOULD NEED TO BE CHANGED BY THE PERSON COMPILING

CFLAGS+=-Iinclude -I${OPENSSL_INCLUDE_DIR} -Wall -Werror
LDFLAGS+=-L${OPENSSL_LIB_DIR} -lcrypto

all: mkdirs headers liboqs tests

OBJECT_DIRS=
include src/common/Makefile
include src/kem/Makefile

HEADERS=src/oqs.h ${HEADERS_COMMON} ${HEADERS_KEM}
OBJECTS=${OBJECTS_COMMON} ${OBJECTS_KEM}
ARCHIVES=${ARCHIVES_KEM}

mkdirs:
	mkdir -p ${OBJECT_DIRS}

DATE=`date`
UNAME=`uname -a`
CC_VERSION=`${CC} --version | tr '\n' ' '`
config_h:
	rm -rf src/config.h
	touch src/config.h
	$(foreach ENABLE_KEM, $(ENABLE_KEMS), echo "#define OQS_ENABLE_KEM_$(ENABLE_KEM)" >> src/config.h;)
	echo "#define OQS_KEM_DEFAULT OQS_KEM_alg_${KEM_DEFAULT}" >> src/config.h
	echo "#define OQS_COMPILE_DATE \"${DATE}\"" >> src/config.h
	echo "#define OQS_COMPILE_CC \"${CC}\"" >> src/config.h
	echo "#define OQS_COMPILE_CC_VERSION \"${CC_VERSION}\"" >> src/config.h
	echo "#define OQS_COMPILE_CFLAGS \"${CFLAGS}\"" >> src/config.h
	echo "#define OQS_COMPILE_LDFLAGS \"${LDFLAGS}\"" >> src/config.h
	echo "#define OQS_COMPILE_ENABLE_KEMS \"${ENABLE_KEMS}\"" >> src/config.h
	echo "#define OQS_COMPILE_KEM_DEFAULT \"${KEM_DEFAULT}\"" >> src/config.h
	echo "#define OQS_COMPILE_UNAME \"${UNAME}\"" >> src/config.h


headers: config_h ${HEADERS}
	rm -rf include
	mkdir -p include/oqs
	cp ${HEADERS} src/config.h include/oqs

liboqs: ${OBJECTS} ${ARCHIVES}
	rm -f liboqs_tmp.a liboqs.a
	ar -r -c liboqs_tmp.a ${OBJECTS}
	libtool -static -o liboqs.a liboqs_tmp.a ${ARCHIVES}
	rm -f liboqs_tmp.a

TEST_PROGRAMS=test_kem
tests: ${TEST_PROGRAMS}

test: tests
	./test_kem

clean:
	rm -rf includes
	rm -rf .objs
	rm -f liboqs.a
	rm -f ${TEST_PROGRAMS}
