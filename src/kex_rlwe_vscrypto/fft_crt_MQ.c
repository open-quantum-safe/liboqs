
#include <assert.h>
#include "fft_crt_MQ.h"

static FFTSHORT modpow(FFTSHORT x, FFTSHORT r, FFTSHORT p) {
  FFTLONG y = 1;
  for (; r; r>>=1) {
    if (r&1) y = (y*x)%p;
    x = (x*(FFTLONG)x)%p;
  }
  return (FFTSHORT)y;
}

static FFTSHORT *get_powers_w(void) {
  static FFTSHORT powers_w[1+BIGMODULUS] = {0};
  if (!powers_w[0]) {   // i.e. if not already initialised
    FFTSHORT i;
    powers_w[0] = 1;
    for (i=2; modpow(i,BIGMODULUS/2,BIGPRIME)!=BIGPRIME-1; i++) ;
    powers_w[1] = i;
    for (i=2; i<=BIGMODULUS; i++) {
      MUL_MOD(powers_w[i],powers_w[i-1],powers_w[1],BIGPRIME);
      if (i<BIGMODULUS) assert(powers_w[i]>=2);
    }
    assert(powers_w[BIGMODULUS]==1);
  }
  return powers_w;
}

/* Gentleman-Sande, decimation-in-frequency fft.
*/
void FUN_MQ(GLOBAL(fft_forward),BIGMODULUS,BIGPRIME)(FFTSHORT *x) {
  FFTSHORT *powers_w = get_powers_w();
  FFTSHORT step;
  FFTSHORT i,j,m;
  FFTSHORT t0,t1;

  for (m=BIGMODULUS>>1,step=1; m >= 1; m=m>>1,step<<=1) {
    for (j = 0 ; j < m; ++j) {
      FFTSHORT power_w = powers_w[BIGMODULUS-j*step];
      for (i = j; i < BIGMODULUS; i += (m<<1)) {
        ADD_MOD(t0, x[i], x[i+m], BIGPRIME);
        t1 = x[i] + BIGPRIME - x[i+m];
        MUL_MOD(x[i+m], t1, power_w, BIGPRIME);
        x[i] = t0;
      }
    }
  }
  return;
}

/* Cooley-Tukey, decimation-in-time fft.
*/
void FUN_MQ(GLOBAL(fft_backward),BIGMODULUS,BIGPRIME)(FFTSHORT *x) {
  FFTSHORT *powers_w = get_powers_w();
  FFTSHORT step;
  FFTSHORT i,j,m;
  FFTSHORT t0,t1;

  for (m=1,step=BIGMODULUS>>1; m<=BIGMODULUS/2; m=m<<1,step>>=1) {
    for (j = 0 ; j < m; ++j) {
      FFTSHORT power_w = powers_w[j*step];
      for (i = j; i < BIGMODULUS; i += (m<<1)) {
        ADD_MOD(t0,x[i],0,BIGPRIME);
        MUL_MOD(t1,x[i+m],power_w,BIGPRIME);
        x[i] = t0 + t1;
        x[i+m] = t0 + BIGPRIME - t1;
      }
    }
  }
  for (i=0; i<BIGMODULUS; i++) x[i] -= (x[i]>=BIGPRIME) ? BIGPRIME : 0;

  return;
}

