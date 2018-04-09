//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <gmp.h>
#include <stdio.h>
#include <memory.h>
#include <sike_params.h>
#include "test_arith.h"
#include "test_sike.h"

int main(int argc, char *argv[]) {

  int rc = 0;

  const char *arg503 = "SIKEp503";
  const char *arg751 = "SIKEp751";
  const char *arg964 = "SIKEp964";

  const char *arg_sidh_int       = "sidh_int";
  const char *arg_arith          = "arith";
  const char *arg_sike           = "sike";
  const char *arg_sike_int       = "sike_int";
  const char *arg_sike_speed     = "sike_speed";
  const char *arg_sike_speed_int = "sike_speed_int";
  const char *arg_pke            = "pke";

  sike_params_t params = { 0 };
  const sike_params_raw_t *params_raw = NULL;


  if (argc < 3) {
    printf("Two arguments needed\n");
    rc = 1;
    goto end;
  }

  if (!strcmp(argv[1], arg503)) {
    params_raw = &SIKEp503;
  } else if (!strcmp(argv[1], arg751)) {
    params_raw = &SIKEp751;
  } else if (!strcmp(argv[1], arg964)) {
    params_raw = &SIKEp964;
  } else {
    rc = 1;
    goto end;
  }

  sike_setup_params(params_raw, &params);

  if (!strcmp(argv[2], arg_arith)) {

    rc = test_arith(&params);
    if (rc) goto end;

  } else if (!strcmp(argv[2], arg_sike)) {

    rc = test_sike(params_raw->name, &params, 1);
    if ( rc ) goto end;

  } else if (!strcmp(argv[2], arg_sike_int)) {

    rc = test_sike_int(params_raw->name, &params);
    if ( rc ) goto end;

  } else if (!strcmp(argv[2], arg_pke)) {

    rc = test_pke(params_raw->name, &params);
    if ( rc ) goto end;

  } else if (!strcmp(argv[2], arg_sike_speed)) {

    if (argc != 4) {
      printf("Need to pass the number of runs\n");
    } else {
      int runs = atoi(argv[3]);
      rc = test_sike_speedy(params_raw->name, &params, runs);
      if ( rc ) goto end;
    }

  } else if (!strcmp(argv[2], arg_sike_speed_int)) {

    if (argc != 4) {
      printf("Need to pass the number of runs\n");
    } else {
      int runs = atoi(argv[3]);
      rc = test_sike_speedy_int(params_raw->name, &params, runs);
      if ( rc ) goto end;
    }

  } else if (!strcmp(argv[2], arg_sidh_int)) {

    rc = test_sidh(params_raw->name, &params);
    if ( rc ) goto end;

  } else {
    printf("Argument not supported\n");
    rc = 1;
  }

end:
  if (params_raw != NULL)
    sike_teardown_params(&params);

  return rc;
}