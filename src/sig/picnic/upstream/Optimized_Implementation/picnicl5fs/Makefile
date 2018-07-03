CC=gcc
CFLAGS+=-fPIC -g -O3
CPPFLAGS+=-I. -Isha3 -DMUL_M4RI -DHAVE_RANDOMBYTES

SOURCES=$(wildcard *.c) $(wildcard sha3/*.c)
#SOURCES=$(filter-out randomness.c,$(wildcard *.c)) $(wildcard sha3/*.c)
EXECUTABLE_APITEST=tests/api_test
EXECUTABLE_NISTKAT=NIST-KATs/PQCgenKAT_sign
EXECUTABLE_EXAMPLE=tests/picnic_test
LIBPICNIC=libpicnic.a
NISTKAT_SOURCES=$(wildcard NIST-KATs/*.c)

all: $(LIBPICNIC) $(EXECUTABLE_APITEST) $(EXECUTABLE_EXAMPLE) $(EXECUTABLE_NISTKAT)

$(LIBPICNIC): $(SOURCES:.c=.o)
	ar rcs $@ $^

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(EXECUTABLE_NISTKAT): CPPFLAGS+=-DHAVE_RANDOMBYTES
$(EXECUTABLE_NISTKAT): CFLAGS+=-Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-result

$(EXECUTABLE_EXAMPLE): $(EXECUTABLE_EXAMPLE).c $(LIBPICNIC) randomness.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(EXECUTABLE_APITEST): $(EXECUTABLE_APITEST).c $(LIBPICNIC) randomness.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(EXECUTABLE_NISTKAT): $(NISTKAT_SOURCES) $(LIBPICNIC) randomness.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -lcrypto -o $@

clean:
	rm -f $(wildcard *.o) $(wildcard sha3/*.o) $(LIBPICNIC) $(EXECUTABLE_APITEST) $(EXECUTABLE_EXAMPLE) $(EXECUTABLE_NISTKAT)
