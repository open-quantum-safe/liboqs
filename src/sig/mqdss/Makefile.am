AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsigmqdss.la
noinst_LTLIBRARIES += libsigmqdss_31_48.la libsigmqdss_31_64.la 

libsigmqdss_la_LIBADD  = libsigmqdss_31_48.la libsigmqdss_31_64.la 
libsigmqdss_la_SOURCES = 

libsigmqdss_31_48_la_SOURCES = sig_mqdss_31_48.c pqclean_mqdss-48_clean/gf31.c pqclean_mqdss-48_clean/mq.c pqclean_mqdss-48_clean/sign.c 
libsigmqdss_31_48_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libsigmqdss_31_64_la_SOURCES = sig_mqdss_31_64.c pqclean_mqdss-64_clean/gf31.c pqclean_mqdss-64_clean/mq.c pqclean_mqdss-64_clean/sign.c 
libsigmqdss_31_64_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

