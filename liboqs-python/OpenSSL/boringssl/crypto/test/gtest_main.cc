/* Copyright (c) 2016, Google Inc.
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

#include <gtest/gtest.h>

#include <stdio.h>

#include <openssl/err.h>
#include <openssl/crypto.h>

namespace {

class ErrorTestEventListener : public testing::EmptyTestEventListener {
 public:
  ErrorTestEventListener() {}
  ~ErrorTestEventListener() override {}

  void OnTestEnd(const testing::TestInfo &test_info) override {
    // If the test failed, print any errors left in the error queue.
    if (test_info.result()->Failed()) {
      ERR_print_errors_fp(stdout);
    }

    // Clean up the error queue for the next run.
    ERR_clear_error();
  }
};

}  // namespace

int main(int argc, char **argv) {
  CRYPTO_library_init();
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(
      new ErrorTestEventListener);
  return RUN_ALL_TESTS();
}
