// SPDX-License-Identifier: MIT

#ifndef OQS_ADAPTER_MACROS_H_
#define OQS_ADAPTER_MACROS_H_

#ifndef KEM_PREFIX
#define KEM_PREFIX common
#endif

#define CONCAT(a, b) a##_##b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)
#define OQS_MAKE_FN(prefix, name) EXPAND_CONCAT(prefix, name)
#define OQS_MAKE_FN_STR(prefix) #prefix

#endif
