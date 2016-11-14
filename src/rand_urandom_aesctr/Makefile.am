AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = librandaesctr.la

librandaesctr_la_SOURCES = /rand_urandom_aesctr.c
librandaesctr_la_CPPFLAGS = -I../../include -I.
librandaesctr_la_CPPFLAGS += $(AM_CPPFLAGS)

#liboqs_la_LDFLAGS = -version-info 0:0:0

test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
