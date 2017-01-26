AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libmsrln16.la


libmsrln16_la_SOURCES = kex_rlwe_msrln16.c LatticeCrypto_kex.c ntt_constants.c
libmsrln16_la_CPPFLAGS = -I../../include -fPIC
libmsrln16_la_CPPFLAGS += $(AM_CPPFLAGS) 

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
