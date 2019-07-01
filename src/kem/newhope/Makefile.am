AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemnewhope.la
noinst_LTLIBRARIES += libkemnewhope_512cca.la libkemnewhope_1024cca.la 

libkemnewhope_la_LIBADD  = libkemnewhope_512cca.la libkemnewhope_1024cca.la 
libkemnewhope_la_SOURCES = 

libkemnewhope_512cca_la_SOURCES = kem_newhope_512cca.c pqclean_newhope_512cca_clean/cpapke.c pqclean_newhope_512cca_clean/kem.c pqclean_newhope_512cca_clean/ntt.c pqclean_newhope_512cca_clean/poly.c pqclean_newhope_512cca_clean/precomp.c pqclean_newhope_512cca_clean/reduce.c pqclean_newhope_512cca_clean/verify.c 
libkemnewhope_512cca_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemnewhope_1024cca_la_SOURCES = kem_newhope_1024cca.c pqclean_newhope_1024cca_clean/cpapke.c pqclean_newhope_1024cca_clean/kem.c pqclean_newhope_1024cca_clean/ntt.c pqclean_newhope_1024cca_clean/poly.c pqclean_newhope_1024cca_clean/precomp.c pqclean_newhope_1024cca_clean/reduce.c pqclean_newhope_1024cca_clean/verify.c 
libkemnewhope_1024cca_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

