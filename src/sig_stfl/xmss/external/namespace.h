// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#ifndef XMSS_NAMESPACE_H
#define XMSS_NAMESPACE_H

#define XMSS__(prefix, funcname) prefix##_##funcname
#define XMSS_(prefix, funcname) XMSS__(prefix, funcname)
#define __XMSS(funcname, postfix) funcname##_##postfix
#define _XMSS(funcname, postfix) __XMSS(funcname, postfix)

#define XMSS_PARAMS _XMSS(oqs_sig_stfl, XMSS_PARAMS_NAMESPACE)
#define XMSS_PARAMS_INNER _XMSS(_XMSS(oqs_sig_stfl, XMSS_PARAMS_NAMESPACE), inner)
#define XMSS_PARAMS_INNER_CORE _XMSS(_XMSS(oqs_sig_stfl, XMSS_PARAMS_NAMESPACE), inner)

#define XMSS_PARAMS_INNER_CORE_HASH(funcname) XMSS_(XMSS_PARAMS_INNER_CORE, funcname)

#define XMSS_NAMESPACE(funcname) XMSS_(XMSS_PARAMS, funcname)
#define XMSS_INNER_NAMESPACE(funcname) XMSS_(XMSS_PARAMS_INNER, funcname)

#endif
