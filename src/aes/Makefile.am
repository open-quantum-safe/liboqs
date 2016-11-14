AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libaes.la

libaes_la_SOURCES = aes.c

if USE_OPENSSL
libaes_la_CPPFLAGS = -DUSE_OPENSSL
libaes_la_CPPFLAGS += -I$(OPENSSL_DIR)/include
else
if USE_AES_NI
libaes_la_CPPFLAGS = -maes -msse2
libaes_la_SOURCES += aes_ni.c
else
libaes_la_CPPFLAGS = -DAES_DISABLE_NI
libaes_la_SOURCES += aes_c.c
endif
endif

libaes_la_CPPFLAGS += -I../../include
libaes_la_CPPFLAGS += $(AM_CPPFLAGS)


test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
