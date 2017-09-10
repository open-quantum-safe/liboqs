AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libnewhope_avx2.la

libnewhope_avx2_la_SOURCES = crypto_stream_chacha20.c poly.c precomp.c error_correction.c
libnewhope_avx2_la_SOURCES += crypto_hash_sha256.c chacha.S cbd.s consts.c fips202.c omegas.c ntt_double.s randombytes.c
libnewhope_avx2_la_SOURCES += bitrev.s crypto_stream_aes256ctr.s hr.s rec.s poly_pointwise.s kex_rlwe_newhope_avx2.c

libnewhope_avx2_la_CPPFLAGS = -I../../../include -I.
libnewhope_avx2_la_CPPFLAGS += $(AM_CPPFLAGS) -O3 -fomit-frame-pointer -msse2avx -mavx2 -march=corei7-avx


