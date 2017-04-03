AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libvscrypto.la

libvscrypto_la_SOURCES = kex_rlwe_vscrypto.c vscrypto_kex.c set_ring_ctx.c

# I don't know how to add libtool objects that are created from the
# same C sources compiled in multiple different ways.  So I've
# simulated the effect with these "messy_*.c" files.  Sorry.
libvscrypto_la_SOURCES += messy_ctx_1024_40961.c messy_ctx_256_15361.c messy_ctx_337_32353.c messy_ctx_433_35507.c messy_ctx_512_25601.c messy_ctx_541_41117.c messy_ctx_631_44171.c messy_ctx_739_47297.c messy_ctx_821_49261.c messy_fft_1024_17633281.c messy_fft_1024_8816641.c messy_fft_2048_17633281.c messy_fft_2048_8816641.c

libvscrypto_la_CPPFLAGS = -I../../include
libvscrypto_la_CPPFLAGS += $(AM_CPPFLAGS) 

