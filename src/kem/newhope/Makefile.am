AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkemnewhope.la
noinst_LTLIBRARIES += libkemnewhope512cca.la libkemnewhope1024cca.la 

libkemnewhope_la_LIBADD  = libkemnewhope512cca.la libkemnewhope1024cca.la 
libkemnewhope_la_SOURCES = 

libkemnewhope512cca_la_SOURCES = kem_newhope512cca.c pqclean_newhope512cca_clean/cpapke.c pqclean_newhope512cca_clean/kem.c pqclean_newhope512cca_clean/ntt.c pqclean_newhope512cca_clean/poly.c pqclean_newhope512cca_clean/precomp.c pqclean_newhope512cca_clean/reduce.c pqclean_newhope512cca_clean/verify.c 
libkemnewhope512cca_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

libkemnewhope1024cca_la_SOURCES = kem_newhope1024cca.c pqclean_newhope1024cca_clean/cpapke.c pqclean_newhope1024cca_clean/kem.c pqclean_newhope1024cca_clean/ntt.c pqclean_newhope1024cca_clean/poly.c pqclean_newhope1024cca_clean/precomp.c pqclean_newhope1024cca_clean/reduce.c pqclean_newhope1024cca_clean/verify.c 
libkemnewhope1024cca_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

