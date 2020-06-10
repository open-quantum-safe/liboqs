/* Copyright (c) 2015, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#ifndef OPENSSL_HEADER_CRYPTO_TEST_TEST_UTIL_H
#define OPENSSL_HEADER_CRYPTO_TEST_TEST_UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <iosfwd>

#include "../internal.h"


// hexdump writes |msg| to |fp| followed by the hex encoding of |len| bytes
// from |in|.
void hexdump(FILE *fp, const char *msg, const void *in, size_t len);

// Bytes is a wrapper over a byte slice which may be compared for equality. This
// allows it to be used in EXPECT_EQ macros.
struct Bytes {
  Bytes(const uint8_t *data_arg, size_t len_arg)
      : data(data_arg), len(len_arg) {}

  Bytes(const char *str)
      : data(reinterpret_cast<const uint8_t *>(str)), len(strlen(str)) {}

  template <size_t N>
  Bytes(const uint8_t (&array)[N]) : data(array), len(N) {}

  const uint8_t *data;
  size_t len;
};

inline bool operator==(const Bytes &a, const Bytes &b) {
  return a.len == b.len && OPENSSL_memcmp(a.data, b.data, a.len) == 0;
}

inline bool operator!=(const Bytes &a, const Bytes &b) { return !(a == b); }

std::ostream &operator<<(std::ostream &os, const Bytes &in);


#endif /* OPENSSL_HEADER_CRYPTO_TEST_TEST_UTIL_H */
