AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libqtesla.la

libqtesla_la_SOURCES = qtesla_p_I.c qtesla_p_III.c sig_qtesla.c

QTESLA_FLAGS =
if X86
QTESLA_FLAGS += -D_X86_
else
if X86_64
QTESLA_FLAGS += -D_AMD64_
else
if ARM
QTESLA_FLAGS += -D_ARM_
else
if ARM64
QTESLA_FLAGS += -D_ARM64_
endif # ARM64
endif # ARM
endif # X86_64
endif # X86

if USE_AVX2_INSTRUCTIONS
if USE_AES_INSTRUCTIONS
QTESLA_FLAGS += -DWITH_AVX2 -Iavx2
libqtesla_la_SOURCES += avx2/poly_mul1024.S avx2/poly_mul2048.S
else
QTESLA_FLAGS += -Iportable
endif # USE_AES_INSTRUCTIONS
else
QTESLA_FLAGS += -Iportable
endif # USE_AVX2_INSTRUCTIONS

libqtesla_la_CFLAGS = $(AM_CFLAGS) $(QTESLA_FLAGS)

