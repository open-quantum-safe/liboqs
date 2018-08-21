AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libaes.la

libaes_la_SOURCES = aes.c
libaes_la_SOURCES += aes_c.c

if USE_AES_NI
libaes_la_SOURCES += aes_ni.c
endif
