AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libbcns15.la


libbcns15_la_SOURCES = fft.c kex_rlwe_bcns15.c rlwe.c rlwe_kex.c

libbcns15_la_CPPFLAGS = -I../../include -fPIC
libbcns15_la_CPPFLAGS += $(AM_CPPFLAGS) 

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
