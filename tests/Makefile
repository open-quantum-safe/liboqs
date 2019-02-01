TEST_PROGRAMS=tests/test_kem tests/test_kem_shared tests/test_sig tests/test_sig_shared
$(TEST_PROGRAMS): liboqs
tests: $(TEST_PROGRAMS)

KAT_PROGRAMS=tests/kat_kem tests/kat_sig
$(KAT_PROGRAMS): liboqs
kats: $(KAT_PROGRAMS)

test: tests
	tests/test_kem
	tests/test_sig

kat: kats
	tests/kat_kem
	tests/kat_sig
	scripts/check_kats.sh

SPEED_PROGRAMS=tests/speed_kem tests/speed_sig
$(SPEED_PROGRAMS): liboqs
speeds: $(SPEED_PROGRAMS)

speed: speeds
	tests/speed_kem --info
	tests/speed_sig --info

EXAMPLE_PROGRAMS=tests/example_kem tests/example_sig
$(EXAMPLE_PROGRAMS): liboqs
examples: $(EXAMPLE_PROGRAMS)

tests/test_kem: headers tests/test_kem.c liboqs
	$(CC) tests/test_kem.c liboqs.a -o tests/test_kem $(CFLAGS) $(LDFLAGS)

tests/test_kem_shared: tests/test_kem.c liboqs
	$(CC) tests/test_kem.c -o tests/test_kem_shared $(CFLAGS) $(LDFLAGS) -L. -loqs -lcrypto -lm

tests/kat_kem: headers tests/kat_kem.c liboqs
	$(CC) tests/kat_kem.c liboqs.a -o tests/kat_kem $(CFLAGS) $(LDFLAGS)

tests/speed_kem: headers tests/speed_kem.c liboqs
	$(CC) tests/speed_kem.c liboqs.a -o tests/speed_kem $(CFLAGS) $(LDFLAGS)

tests/example_kem: headers tests/example_kem.c liboqs
	$(CC) tests/example_kem.c liboqs.a -o tests/example_kem $(CFLAGS) $(LDFLAGS)

tests/test_sig: headers tests/test_sig.c liboqs
	$(CC) tests/test_sig.c liboqs.a -o tests/test_sig $(CFLAGS) $(LDFLAGS)

tests/test_sig_shared: tests/test_sig.c liboqs
	$(CC) tests/test_sig.c -o tests/test_sig_shared $(CFLAGS) $(LDFLAGS) -L. -loqs -lcrypto -lm

tests/kat_sig: headers tests/kat_sig.c liboqs
	$(CC) tests/kat_sig.c liboqs.a -o tests/kat_sig $(CFLAGS) $(LDFLAGS)

tests/speed_sig: headers tests/speed_sig.c liboqs
	$(CC) tests/speed_sig.c liboqs.a -o tests/speed_sig $(CFLAGS) $(LDFLAGS)

tests/example_sig: headers tests/example_sig.c liboqs
	$(CC) tests/example_sig.c liboqs.a -o tests/example_sig $(CFLAGS) $(LDFLAGS)
