AUTOMAKE_OPTIONS = foreign
noinst_bin_PROGRAMS = test_rand
noinst_bindir=$(prefix)/tests

test_rand_SOURCES = test_rand.c rand.c ../rand_urandom_chacha20/rand_urandom_chacha20.c
test_rand_CPPFLAGS = -I../../include
test_rand_CPPFLAGS += $(AM_CPPFLAGS)


test:
	$(foreach tp,$(noinst_bin_PROGRAMS),./$(tp) || exit;)
