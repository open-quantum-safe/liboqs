#include "sdith_signature.h"

const signature_parameters CAT1_SHORT_PARAMETERS = {
    //
    .lambda = 128,               //
    .kappa = 11,                 //
    .tau = 11,                   //
    .target_topen = 109,         //
    .proofow_w = 9,              //
    .rsd_w = 56,                 //
    .rsd_n = 10360,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
};

const signature_parameters CAT3_SHORT_PARAMETERS = {
    //
    .lambda = 192,               //
    .kappa = 11,                 //
    .tau = 17,                   //
    .target_topen = 169,         //
    .proofow_w = 7,              //
    .rsd_w = 73,                 //
    .rsd_n = 18396,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 4},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
  };

const signature_parameters CAT5_SHORT_PARAMETERS = {
    //
    .lambda = 256,               //
    .kappa = 11,                 //
    .tau = 23,                   //
    .target_topen = 228,         //
    .proofow_w = 5,              //
    .rsd_w = 104,                //
    .rsd_n = 19864,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
};

const signature_parameters CAT1_FAST_PARAMETERS = {
    //
    .lambda = 128,               //
    .kappa = 7,                  //
    .tau = 18,                   //
    .target_topen = 107,         //
    .proofow_w = 4,              //
    .rsd_w = 56,                 //
    .rsd_n = 10360,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
};

const signature_parameters CAT3_FAST_PARAMETERS = {
    //
    .lambda = 192,               //
    .kappa = 7,                  //
    .tau = 27,                   //
    .target_topen = 161,         //
    .proofow_w = 5,              //
    .rsd_w = 73,                 //
    .rsd_n = 18396,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 4},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
};

const signature_parameters CAT5_FAST_PARAMETERS = {
    //
    .lambda = 256,               //
    .kappa = 7,                  //
    .tau = 36,                   //
    .target_topen = 216,         //
    .proofow_w = 6,              //
    .rsd_w = 104,                //
    .rsd_n = 19864,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_SHAKE //
};

// The cipher-grinding variants. kappa and the RSD parameters are shared with
// the base set (so pk and sk are identical), but do NOT just copy the base set
// and flip the variant: in round 3, cat1-fast and cat3-fast use their own tau,
// target_topen and proofow_w (see the specs, tab:params-voleith-aespow). The
// short sets and cat5-fast keep the base values, so only the variant differs
// for those.

const signature_parameters CAT1_SHORT_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 128,               //
    .kappa = 11,                 //
    .tau = 11,                   //
    .target_topen = 109,         //
    .proofow_w = 9,              //
    .rsd_w = 56,                 //
    .rsd_n = 10360,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
};

const signature_parameters CAT3_SHORT_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 192,               //
    .kappa = 11,                 //
    .tau = 17,                   //
    .target_topen = 169,         //
    .proofow_w = 7,              //
    .rsd_w = 73,                 //
    .rsd_n = 18396,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 4},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
  };

const signature_parameters CAT5_SHORT_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 256,               //
    .kappa = 11,                 //
    .tau = 23,                   //
    .target_topen = 228,         //
    .proofow_w = 5,              //
    .rsd_w = 104,                //
    .rsd_n = 19864,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
};

const signature_parameters CAT1_FAST_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 128,               //
    .kappa = 7,                  //
    .tau = 17,                   //
    .target_topen = 101,         //
    .proofow_w = 11,             //
    .rsd_w = 56,                 //
    .rsd_n = 10360,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
};

const signature_parameters CAT3_FAST_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 192,               //
    .kappa = 7,                  //
    .tau = 26,                   //
    .target_topen = 155,         //
    .proofow_w = 12,             //
    .rsd_w = 73,                 //
    .rsd_n = 18396,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 4},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
};

const signature_parameters CAT5_FAST_CIPHERPOW_PARAMETERS = {
    //
    .lambda = 256,               //
    .kappa = 7,                  //
    .tau = 36,                   //
    .target_topen = 216,         //
    .proofow_w = 6,              //
    .rsd_w = 104,                //
    .rsd_n = 19864,              //
    .mux_depth = 4,              //
    .mux_arities = {4, 4, 4, 3},  //
    .proofow_variant = PROOFOW_VARIANT_CIPHER //
};
