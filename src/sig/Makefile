HEADERS_SIG=src/sig/sig.h

OBJECT_DIRS+=.objs/sig
OBJECTS_SIG=.objs/sig/sig.o

.objs/sig/sig.o: headers src/sig/sig.c
	$(CC) -c src/sig/sig.c -o .objs/sig/sig.o $(CFLAGS)

test_sig: headers src/sig/test_sig.c liboqs
	$(CC) src/sig/test_sig.c liboqs.a -o test_sig $(CFLAGS) $(LDFLAGS)

test_sig_shared: src/sig/test_sig.c liboqs
	$(CC) src/sig/test_sig.c -o test_sig_shared $(CFLAGS) $(LDFLAGS) -L. -loqs -lcrypto -lm

kat_sig: headers src/sig/kat_sig.c liboqs
	$(CC) src/sig/kat_sig.c liboqs.a -o kat_sig $(CFLAGS) $(LDFLAGS)

speed_sig: headers src/sig/speed_sig.c liboqs
	$(CC) src/sig/speed_sig.c liboqs.a -o speed_sig $(CFLAGS) $(LDFLAGS)

example_sig: headers src/sig/example_sig.c liboqs
	$(CC) src/sig/example_sig.c liboqs.a -o example_sig $(CFLAGS) $(LDFLAGS)

include src/sig/picnic/Makefile
include src/sig/qtesla/Makefile
# EDIT-WHEN-ADDING-SIG
