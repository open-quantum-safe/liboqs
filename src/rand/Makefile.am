AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = librand.la

librand_la_SOURCES = rand.c

librand_la_CPPFLAGS = -I../../include
librand_la_CPPFLAGS += $(AM_CPPFLAGS)

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
