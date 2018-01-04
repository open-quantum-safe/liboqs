CC=gcc
OPENSSL_INCLUDE_DIR=/usr/local/opt/openssl/include
OPENSSL_LIB_DIR=/usr/local/opt/openssl/lib
CFLAGS=-Iinclude -I${OPENSSL_INCLUDE_DIR} -Wall -Werror
LDFLAGS=-L{OPENSSL_LIB_DIR} -lcrypto

all: headers liboqs

include src/common/Makefile
include src/kem/Makefile

HEADERS=src/oqs.h ${HEADERS_COMMON} ${HEADERS_KEM}
OBJECTS=${OBJECTS_COMMON} ${OBJECTS_KEM}

headers: ${HEADERS}
	rm -rf include
	mkdir -p include/oqs
	cp ${HEADERS} include/oqs

liboqs: common kem
	rm -f liboqs.a
	ar -r -c liboqs.a ${OBJECTS}

clean: common_clean kem_clean
	rm -rf includes
	rm -rf .objs
