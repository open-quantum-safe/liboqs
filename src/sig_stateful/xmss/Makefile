CXX = gcc
CXX_FLAGS = -g -Wall -O0
EXEC = xmss
LDLIBS = -lcrypto -loqs

SOURCES = params.c hash.c fips202.c hash_address.c randombytes.c wots.c xmss.c xmss_core.c xmss_commons.c utils.c
HEADERS = params.h hash.h fips202.h hash_address.h randombytes.h wots.h xmss.h xmss_core.h xmss_commons.h utils.h
# OBJECTS = params.o hash.o fips202.o hash_address.o randombytes.o wots.o xmss.o xmss_core.o xmss_commons.o utils.o
# DEPENDS = ${OBJECTS:.o=.d}

SOURCES_FAST = $(subst xmss_core.c,xmss_core_fast.c,$(SOURCES))
HEADERS_FAST = $(subst xmss_core.c,xmss_core_fast.c,$(HEADERS))

TESTS = tests/test_full tests/test_step

tests: $(TESTS)

.PHONY: clean test

tests/test_full: tests/complete_tester.c $(SOURCES_FAST) $(OBJS) $(HEADERS_FAST)
	$(CXX) $(CXX_FLAGS) -o $@ $(SOURCES_FAST) $< $(LDLIBS) -L../../../build/lib -I../../../build/include -I.

tests/test_step: tests/interactive_tester.c $(SOURCES_FAST) $(OBJS) $(HEADERS_FAST)
	$(CXX) $(CXX_FLAGS) -o $@ $(SOURCES_FAST) $< $(LDLIBS) -L../../../build/lib -I../../../build/include -I.

clean:
	-$(RM) $(TESTS)