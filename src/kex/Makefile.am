AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libkex.la

libkex_la_SOURCES = kex.c
libkex_la_SOURCES += ../rand/rand.c
libkex_la_SOURCES += ../rand_urandom_chacha20/rand_urandom_chacha20.c

libkex_la_CPPFLAGS = -I../../include

#liboqs_la_LDFLAGS = -version-info 0:0:0

noinst_bin_PROGRAMS = test_kex
noinst_bindir=$(prefix)/tests
test_kex_SOURCES = test_kex.c
test_kex_CPPFLAGS = -I../../include
test_kex_LDADD =  libkex.la
test_kex_LDADD += ../kex_rlwe_bcns15/libbcns15.la

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
