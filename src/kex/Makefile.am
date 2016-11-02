AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkex.la

libkex_la_SOURCES = kex.c

libkex_la_CPPFLAGS = -I../../include
libkex_la_CPPFLAGS += $(AM_CPPFLAGS)

#liboqs_la_LDFLAGS = -version-info 0:0:0

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
