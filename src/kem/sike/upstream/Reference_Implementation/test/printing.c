//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <stdio.h>
#include <fp2.h>
#include <montgomery.h>

void print_hex(const char* title, const unsigned char* seq, size_t seqLen) {
  printf("%s: ", title);
  for (int i = 0; i < seqLen; ++i) {
    printf("%02X", seq[i]);
  }
  printf("\n");
}

void printFp2(const char* title, const fp2* f) {
  gmp_printf("%s\np = %Zu*i + %Zu\n", title, f->x1, f->x0);
}

void printPt(const char* title, const mont_pt_t* pt) {
  gmp_printf("%s\npx = %Zu * i + %Zu\npy = %Zu * i + %Zu\n", title, pt->x.x1, pt->x.x0, pt->y.x1, pt->y.x0);
}

void printPk(const char* title, const mont_curve_int_t* curve) {
  printf("%s\n", title);
  printPt("P = ", &curve->P);
  printPt("Q = ", &curve->Q);
  printFp2("coeff.a = ", &curve->a);
  printFp2("coeff.b = ", &curve->b);
}

void printPkExt(const char* title, const sike_public_key_t* curve) {
  printf("%s\n", title);
  printFp2("xP = ", &curve->xP);
  printFp2("xQ = ", &curve->xQ);
  printFp2("xR = ", &curve->xR);
}