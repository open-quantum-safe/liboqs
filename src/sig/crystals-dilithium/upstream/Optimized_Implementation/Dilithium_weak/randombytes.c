#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "randombytes.h"

#define _GNU_SOURCE

static int fd = -1;
static void randombytes_fallback(unsigned char *x, size_t xlen)
{
  int i;

  if (fd == -1) {
    for (;;) {
      fd = open("/dev/urandom", O_RDONLY);
      if (fd != -1) break;
      sleep(1);
    }
  }

  while (xlen > 0) {
    if (xlen < 1048576) i = xlen; else i = 1048576;

    i = read(fd, x, i);
    if (i < 1) {
      sleep(1);
      continue;
    }

    x += i;
    xlen -= i;
  }
}

#ifdef SYS_getrandom
void randombytes(unsigned char *buf, size_t buflen)
{
  size_t d = 0;
  int r;

  while(d < buflen)
  {
    r = syscall(SYS_getrandom, buf, buflen, 0);
    if(r < 0)
    {
      randombytes_fallback(buf, buflen);
      return;
    }
    buf += r;
    d += r;
  }
}
#else
void randombytes(unsigned char *buf, size_t buflen)
{
  randombytes_fallback(buf, buflen);
}
#endif

