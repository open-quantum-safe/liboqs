AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libsha2.la

libsha2_la_SOURCES =
if USE_SHA2_OPENSSL
libsha2_la_SOURCES += sha2_ossl.c
else
libsha2_la_SOURCES += sha2_c.c
endif
