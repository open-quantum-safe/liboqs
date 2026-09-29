/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "randomness.h"
#include "macros.h"

#if defined(HAVE_RANDOMBYTES) || defined(SUPERCOP)
#if defined(PQCLEAN)
// randombytes from the PQClean
extern void randombytes(uint8_t* x, size_t xlen);
#else
// randombytes from the NIST framework / SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#endif

int rand_bytes(uint8_t* dst, size_t len) {
  randombytes(dst, len);
  return 0;
}
#elif defined(OQS)
#include <oqs/rand.h>

int rand_bytes(uint8_t* dst, size_t len) {
  OQS_randombytes(dst, len);
  return 0;
}
#else

#if (defined(HAVE_SYS_RANDOM_H) && defined(HAVE_GETRANDOM)) ||                                     \
    (defined(__linux__) && GLIBC_CHECK(2, 25))
#include <sys/random.h>

int rand_bytes(uint8_t* dst, size_t len) {
  const ssize_t ret = getrandom(dst, len, GRND_NONBLOCK);
  if (ret < 0 || (size_t)ret != len) {
    return -1;
  }
  return 0;
}
#elif defined(HAVE_ARC4RANDOM_BUF)
#include <stdlib.h>

int rand_bytes(uint8_t* dst, size_t len) {
  arc4random_buf(dst, len);
  return 0;
}
#elif defined(__APPLE__) && defined(HAVE_APPLE_FRAMEWORK)
#include <Security/Security.h>

int rand_bytes(uint8_t* dst, size_t len) {
  if (SecRandomCopyBytes(kSecRandomDefault, len, dst) == errSecSuccess) {
    return 0;
  }
  return -1;
}
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NETBSD__) ||   \
    defined(__NetBSD__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#if defined(__linux__)
#include <linux/random.h>
#include <sys/ioctl.h>
#endif

#if !defined(O_NOFOLLOW)
#define O_NOFOLLOW 0
#endif
#if !defined(O_CLOEXEC)
#define O_CLOEXEC 0
#endif

int rand_bytes(uint8_t* dst, size_t len) {
  int fd;
  while ((fd = open("/dev/urandom", O_RDONLY | O_NOFOLLOW | O_CLOEXEC, 0)) == -1) {
    // check if we should restart
    if (errno != EINTR) {
      return -1;
    }
  }
#if O_CLOEXEC == 0
  fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
#endif

#if defined(__linux__)
  int cnt = 0;
  if (ioctl(fd, RNDGETENTCNT, &cnt) == -1) {
    // not ready
    close(fd);
    return -1;
  }
#endif

  while (len) {
    const ssize_t ret = read(fd, dst, len);
    if (ret == -1) {
      if (errno == EAGAIN || errno == EINTR) {
        // retry
        continue;
      }
      close(fd);
      return -1;
    }

    dst += ret;
    len -= ret;
  }

  close(fd);
  return 0;
}
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>

int rand_bytes(uint8_t* dst, size_t len) {
  if (len > ULONG_MAX) {
    return -1;
  }
  if (!BCRYPT_SUCCESS(BCryptGenRandom(NULL, dst, (ULONG)len, BCRYPT_USE_SYSTEM_PREFERRED_RNG))) {
    return -1;
  }
  return 0;
}
#else
#error "Unsupported OS! Please implement rand_bytes."
#endif
#endif
