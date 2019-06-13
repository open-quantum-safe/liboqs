AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libdilithium.la
libdilithium_la_LIBADD=
libdilithium_la_SOURCES = sig_dilithium.c
libdilithium_la_CFLAGS = $(AM_CFLAGS)


noinst_LTLIBRARIES += libdilithium2.la
libdilithium_la_LIBADD += libdilithium2.la

libdilithium2_la_SOURCES = external/dilithium2/clean/ntt.c external/dilithium2/clean/packing.c external/dilithium2/clean/poly.c external/dilithium2/clean/polyvec.c external/dilithium2/clean/reduce.c external/dilithium2/clean/rounding.c external/dilithium2/clean/sign.c external/dilithium2/clean/symmetric.c
libdilithium2_la_CFLAGS = $(AM_CFLAGS) -I../../common/pqclean_shims



noinst_LTLIBRARIES += libdilithium3.la
libdilithium_la_LIBADD += libdilithium3.la

libdilithium3_la_SOURCES = external/dilithium3/clean/ntt.c external/dilithium3/clean/packing.c external/dilithium3/clean/poly.c external/dilithium3/clean/polyvec.c external/dilithium3/clean/reduce.c external/dilithium3/clean/rounding.c external/dilithium3/clean/sign.c external/dilithium3/clean/symmetric.c
libdilithium3_la_CFLAGS = $(AM_CFLAGS) -I../../common/pqclean_shims


noinst_LTLIBRARIES += libdilithium4.la
libdilithium_la_LIBADD += libdilithium4.la

libdilithium4_la_SOURCES = external/dilithium4/clean/ntt.c external/dilithium4/clean/packing.c external/dilithium4/clean/poly.c external/dilithium4/clean/polyvec.c external/dilithium4/clean/reduce.c external/dilithium4/clean/rounding.c external/dilithium4/clean/sign.c external/dilithium4/clean/symmetric.c
libdilithium4_la_CFLAGS = $(AM_CFLAGS) -I../../common/pqclean_shims


