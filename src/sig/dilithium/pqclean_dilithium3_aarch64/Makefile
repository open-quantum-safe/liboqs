# This Makefile can be used with GNU Make or BSD Make

LIB=libdilithium3_aarch64.a
HEADERS=api.h fips202x2.h macros_common.inc macros.inc NTT_params.h ntt.h packing.h params.h poly.h polyvec.h reduce.h rounding.h sign.h symmetric.h
OBJECTS=fips202x2.o ntt.o packing.o poly.o polyvec.o reduce.o rounding.o sign.o symmetric-shake.o __asm_iNTT.o __asm_NTT.o __asm_poly.o


CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS) -g

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS) $(HEADERS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
