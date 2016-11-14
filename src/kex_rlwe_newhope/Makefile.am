AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libnewhope.la

libnewhope_la_SOURCES = fips202.c kex_rlwe_newhope.c newhope.c precomp.c poly.c

libnewhope_la_CPPFLAGS = -I../../include -I.
libnewhope_la_CPPFLAGS += $(AM_CPPFLAGS)

#liboqs_la_LDFLAGS = -version-info 0:0:0

test:
