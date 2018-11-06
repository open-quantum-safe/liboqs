/**
 * \file oqs.h
 * \brief Overall header file for liboqs.
 *
 * C programs using liboqs can include just this one file, and it will include all
 * other necessary headers from liboqs.
 */

#ifndef __OQS_H
#define __OQS_H

#if !defined(_WIN32)
#include <oqs/oqsconfig.h>
#else
#include <oqs/winconfig.h>
#endif

#include <oqs/common.h>
#include <oqs/aes.h>
#include <oqs/sha3.h>
#include <oqs/rand.h>
#include <oqs/kem.h>
#include <oqs/sig.h>

#endif // __OQS_H
