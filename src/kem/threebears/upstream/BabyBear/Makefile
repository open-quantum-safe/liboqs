CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -Os -std=c11
BUILDDIR ?= build

SRCS = $(wildcard *.c) $(wildcard NIST/*.c)
SRCS2 = $(subst NIST/,,$(SRCS:.c=.o))
OBJS = $(addprefix $(BUILDDIR)/,$(SRCS2))

.PHONY: clean all kat

all: $(BUILDDIR)/genkat
	
kat: $(BUILDDIR)/genkat
	(cd $(BUILDDIR) && ./genkat)

$(BUILDDIR)/%.o: %.c *.h
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I. -INIST -c -o $@ $<

$(BUILDDIR)/%.o: NIST/%.c *.h NIST/*.h
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I. -INIST -c -o $@ $<

$(BUILDDIR)/genkat: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lkeccak -lcrypto

clean:
	rm -fr $(BUILDDIR)
