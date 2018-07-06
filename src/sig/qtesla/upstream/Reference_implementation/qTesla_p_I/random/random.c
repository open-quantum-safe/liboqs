/******************************************************
* Hardware-based random number generation function
*******************************************************/ 

#include "random.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define _GNU_SOURCE
static int lock = -1;


static __inline void delay(unsigned int count)
{
  while (count--) {}
}


static void randombytes_internal(unsigned char* random_array, unsigned int nbytes)
{ // Generation of "nbytes" of random values
  int r, n = nbytes, count = 0;
    
  if (lock == -1) {
    do {
      lock = open("/dev/urandom", O_RDONLY);
      if (lock == -1) {
        delay(0xFFFFF);
      }
    } while (lock == -1);
  }

  while (n > 0) {
    do {
      r = read(lock, random_array+count, n);
      if (r == -1) {
        delay(0xFFFF);
      }
    } while (r == -1);
    count += r;
    n -= r;
  }
}

#ifdef SYS_getrandom

void randombytes(unsigned char* random_array, unsigned int nbytes)
{ // Generation of "nbytes" of random values
  // Randombytes using system call

  if (syscall(SYS_getrandom, random_array, (size_t)nbytes, 0) < 0)
    randombytes_internal(random_array, nbytes);
}

#else

void randombytes(unsigned char* random_array, unsigned int nbytes)
{
  randombytes_internal(random_array, nbytes);
}

#endif

