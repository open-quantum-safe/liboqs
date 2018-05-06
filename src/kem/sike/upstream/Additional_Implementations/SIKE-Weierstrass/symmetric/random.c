/********************************************************************************************
* Hardware-based random number generation function using /dev/urandom
*********************************************************************************************/

#include <random.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
static int lock = -1;


static __inline void delay(unsigned int count)
{
  while (count--) {}
}


int randombytes(unsigned char* random_array, unsigned long long xlen)
{ // Generation of "nbytes" of random values
  long long r, n = (long long) xlen, count = 0;

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
      r = read(lock, random_array+count, (size_t) n);
      if (r == -1) {
        delay(0xFFFF);
      }
    } while (r == -1);
    count += r;
    n -= r;
  }

  return 0;
}