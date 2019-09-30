AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libqtesla.la

if X86
QTESLA_FLAGS = -D_X86_
else
if X86_64
QTESLA_FLAGS = -D_AMD64_
else
if ARM
QTESLA_FLAGS = -D_ARM_
else
if ARM64
QTESLA_FLAGS = -D_ARM64_
endif # ARM64
endif # ARM
endif # X86_64
endif # X86


libqtesla_la_SOURCES = qtesla_p_I.c qtesla_p_III.c sig_qtesla.c
libqtesla_la_CFLAGS = $(AM_CFLAGS) $(QTESLA_FLAGS) -Iportable

