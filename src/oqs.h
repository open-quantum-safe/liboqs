/**
 * \file oqs.h
 * \brief Overall header file for liboqs.
 *
 * C programs using liboqs can include just this one file, and it will include all
 * other necessary headers from liboqs.
 */

#ifndef OQS_H
#define OQS_H

#if !defined(_WIN32)
#include <oqs/oqsconfig.h>
#else
#include <oqs/winconfig.h>
#endif

#include <oqs/common.h>
#include <oqs/aes.h>
#include <oqs/sha2.h>
#include <oqs/sha3.h>
#include <oqs/rand.h>
#include <oqs/kem.h>
#include <oqs/sig.h>

#endif // OQS_H
