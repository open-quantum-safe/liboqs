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
#include <oqs/config.h>
#endif

#include <oqs/common.h>
#include <oqs/kem.h>
#include <oqs/rand.h>

#endif // __OQS_H
