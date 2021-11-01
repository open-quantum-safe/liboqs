# This Makefile can be used with GNU Make or BSD Make

LIB=libkyber1024_aarch64.a
HEADERS=api.h cbd.h fips202x2.h indcpa.h kem.h macros_common.inc macros.inc NTT_params.h ntt.h params.h poly.h polyvec.h reduce.h rejsample.h symmetric.h verify.h 
OBJECTS=cbd.o fips202x2.o indcpa.o kem.o neon_poly.o neon_polyvec.o neon_symmetric-shake.o ntt.o poly.o polyvec.o reduce.o rejsample.o symmetric-shake.o verify.o __asm_base_mul.o __asm_NTT.o __asm_iNTT.o __asm_poly.o 

CFLAGS=-O3 -Wall -Wextra -Wpedantic -Werror -Wmissing-prototypes -Wredundant-decls -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
