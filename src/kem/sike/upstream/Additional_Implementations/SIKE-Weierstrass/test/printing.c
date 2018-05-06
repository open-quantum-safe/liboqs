//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <stdio.h>
#include <fp2.h>
#include <weierstrass.h>

void print_hex(const char* title, const unsigned char* seq, size_t seqLen) {
  printf("%s: ", title);
  for (int i = 0; i < seqLen; ++i) {
    printf("%02x", seq[i]);
  }
  printf("\n");
}

void printFp2(const char* title, const fp2* f) {
  gmp_printf("%s\np = %Zu + %Zu * i\n", title, f->x0, f->x1);
}

void printPt(const char* title, const weier_pt_t* pt) {
  gmp_printf("%s\npx = %Zu + %Zu * i\npy = %Zu + %Zu * i\n", title, pt->x.x0, pt->x.x1, pt->y.x0, pt->y.x1);
}

void printPk(const char* title, const weier_curve_t* curve) {
  printf("%s\n", title);
  printPt("P = ", &curve->P);
  printPt("Q = ", &curve->Q);
  printFp2("coeff.a = ", &curve->a);
  printFp2("coeff.b = ", &curve->b);
}
