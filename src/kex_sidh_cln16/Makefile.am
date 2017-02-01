AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libcln16.la


libcln16_la_SOURCES = ec_isogeny.c fpx.c kex_sidh_cln16.c SIDH.c sidh_kex.c SIDH_setup.c validate.c AMD64/fp_x64_asm.S
libcln16_la_CPPFLAGS = -I../../include -I.-fPIC
libcln16_la_CPPFLAGS += $(AM_CPPFLAGS) 

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
