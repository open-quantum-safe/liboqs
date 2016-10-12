AUTOMAKE_OPTIONS = foreign 
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = doxygen.cfg

SUBDIRS = ${SRCDIR}

BUILT_SOURCES = link

lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD = ${WANTEDLIBS}

test:
	$(foreach s,$(SUBDIRS),make test -s -C $(s);)

link:
	$(MKDIR_P) include/oqs
	$(LN_S) -f ../../src/kex/kex.h include/oqs
	$(LN_S) -f ../../src/rand/rand.h include/oqs
	$(LN_S) -f ../../src/rand_urandom_chacha20/rand_urandom_chacha20.h include/oqs
	$(LN_S) -f ../../src/kex_rlwe_bcns15/kex_rlwe_bcns15.h include/oqs
