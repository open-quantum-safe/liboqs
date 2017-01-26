AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libsha3.la

libsha3_la_SOURCES = sha3.c

libsha3_la_CPPFLAGS = -I../../../include -I.
libsha3_la_CPPFLAGS += $(AM_CPPFLAGS)

#liboqs_la_LDFLAGS = -version-info 0:0:0

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
