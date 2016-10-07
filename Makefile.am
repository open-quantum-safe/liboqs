AUTOMAKE_OPTIONS = foreign 
ACLOCAL_AMFLAGS = -I config

# DOXYGEN SUPPORT
include $(top_srcdir)/aminclude.am

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = doxygen.cfg

SUBDIRS = ${SRCDIR}

lib_LTLIBRARIES = liboqs.la
liboqs_la_SOURCES =
liboqs_la_LIBADD = ${WANTEDLIBS}

test:
	$(foreach s,$(SUBDIRS),make test -s -C $(s);)
