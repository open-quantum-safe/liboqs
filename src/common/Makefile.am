AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = common.c rand.c rand_nist.c

libcommon_la_CPPFLAGS = -I../../include
libcommon_la_CPPFLAGS += $(AM_CPPFLAGS)
if USE_OPENSSL
libcommon_la_CPPFLAGS += -I$(OPENSSL_DIR)/include
endif
