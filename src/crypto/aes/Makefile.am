AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libaes.la

if USE_AES_OPENSSL
libaes_la_SOURCES = aes_ossl.c
else
libaes_la_SOURCES = aes_c.c
endif
