CXX = gcc
CXX_FLAGS = -g -Wall -MMD
EXEC = xmss
LDLIBS = -lcrypto

SOURCES = params.c hash.c fips202.c hash_address.c randombytes.c wots.c xmss.c xmss_core.c xmss_commons.c utils.c
HEADERS = params.h hash.h fips202.h hash_address.h randombytes.h wots.h xmss.h xmss_core.h xmss_commons.h utils.h
OBJECTS = params.o hash.o fips202.o hash_address.o randombytes.o wots.o xmss.o xmss_core.o xmss_commons.o utils.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXX_FLAGS} test/tester.c ${OBJECTS} -o ${EXEC}

.PHONY: clean

clean:	
	rm ${OBJECTS} ${EXEC} ${DEPENDS}