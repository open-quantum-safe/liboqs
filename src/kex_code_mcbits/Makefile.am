AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libmcbits.la

libmcbits_la_SOURCES = external/operations.c kex_code_mcbits.c

libmcbits_la_CPPFLAGS = -I../../include

libmcbits_la_CPPFLAGS += $(AM_CPPFLAGS)

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
