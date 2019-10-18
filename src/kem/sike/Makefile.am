AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkemsike.la

libkemsike_la_SOURCES = kem_sike.c P434/P434.c P434/P434_compressed.c P503/P503.c P503/P503_compressed.c P610/P610.c P610/P610_compressed.c P751/P751.c P751/P751_compressed.c

AM_CPPFLAGS =
SIKE_FLAGS =
if X86
SIKE_FLAGS += -D_GENERIC_ -D_X86_
else
if X86_64
libkemsike_la_SOURCES += P434/AMD64/fp_x64_asm.S P503/AMD64/fp_x64_asm.S P610/AMD64/fp_x64_asm.S P751/AMD64/fp_x64_asm.S
SIKE_FLAGS += -D_AMD64_
if USE_BMI2_INSTRUCTIONS
# preprocessed assembly files (.S) are passed CPP flags
AM_CPPFLAGS += -D_MULX_
endif #USE_BMI2_INSTRUCTIONS
else
if ARM
SIKE_FLAGS += -D_GENERIC_ -D_ARM_
else
if ARM64
libkemsike_la_SOURCES += P503/ARM64/fp_arm64_asm.S P751/ARM64/fp_arm64_asm.S
SIKE_FLAGS += -D_ARM64_
if USE_BMI2_INSTRUCTIONS
# preprocessed assembly files (.S) are passed CPP flags
AM_CPPFLAGS += -D_MULX_
endif #USE_BMI2_INSTRUCTIONS
endif # ARM64
endif # ARM
endif # X86_64
endif # X86

libkemsike_la_CFLAGS = $(AM_CFLAGS) $(SIKE_FLAGS) -w

P434/P434.c: sike.c sidh.c
P434/P434_compressed.c: sike.c sidh.c
P503/P503.c: sike.c sidh.c
P503/P503_compressed.c: sike.c sidh.c
P610/P610.c: sike.c sidh.c
P610/P610_compressed.c: sike.c sidh.c
P751/P751.c: sike.c sidh.c
P751/P751_compressed.c: sike.c sidh.c
