#ifndef XMSS_NAMESPACE_H
#define XMSS_NAMESPACE_H


#define XMSS_PARAMS oqs_sig_stfl_xmss
#define XMSS_PARAMS_INNER oqs_sig_stfl_xmss_inner

#define XMSS_NAMESPACE(funcname) XMSS_(XMSS_PARAMS, funcname)
#define XMSS_INNER_NAMESPACE(funcname) XMSS_(XMSS_PARAMS_INNER, funcname)
#define XMSS_(prefix, funcname) XMSS__(prefix, funcname)
#define XMSS__(prefix, funcname) prefix ## _ ## funcname


#endif
