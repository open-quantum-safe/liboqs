AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libntru.la


libntru_la_SOURCES = kex_ntru.c
libntru_la_CPPFLAGS = -I../../include -I../../external/NTRUEncrypt-master/include
libntru_la_CPPFLAGS += $(AM_CPPFLAGS) 
test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
