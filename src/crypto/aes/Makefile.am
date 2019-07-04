AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libaes.la

libaes_la_SOURCES = 

if USE_AES_NI
libaes_la_SOURCES += aes_ni.c
else
if USE_AES_OPENSSL
libaes_la_SOURCES += aes_ossl.c
else
libaes_la_SOURCES += aes_c.c
endif
endif
