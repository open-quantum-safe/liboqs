ENABLE_KEMS=dummy

CC=gcc
OPENSSL_INCLUDE_DIR=/usr/local/opt/openssl/include
OPENSSL_LIB_DIR=/usr/local/opt/openssl/lib
CFLAGS=-Iinclude -I${OPENSSL_INCLUDE_DIR} -Wall -Werror
LDFLAGS=-L${OPENSSL_LIB_DIR} -lcrypto

all: mkdirs headers liboqs tests

OBJECT_DIRS=
include src/common/Makefile
include src/kem/Makefile

HEADERS=src/oqs.h ${HEADERS_COMMON} ${HEADERS_KEM}
OBJECTS=${OBJECTS_COMMON} ${OBJECTS_KEM}
ARCHIVES=${ARCHIVES_KEM}

mkdirs:
	mkdir -p ${OBJECT_DIRS}

config_h:
	rm -rf src/config.h
	touch src/config.h
	$(foreach ENABLE_KEM, $(ENABLE_KEMS), echo "#define OQS_ENABLE_KEM_$(ENABLE_KEM)" >> src/config.h;)

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

clean:
	rm -rf includes
	rm -rf .objs
	rm -f liboqs.a
	rm -f ${TEST_PROGRAMS}
