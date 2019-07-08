AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libsha3.la

libsha3_la_SOURCES = sha3_c.c
if USE_SHA3_OPENSSL
libsha3_la_SOURCES += sha3_ossl.c
endif

sha3_c.c: fips202.c sp800-185.c
