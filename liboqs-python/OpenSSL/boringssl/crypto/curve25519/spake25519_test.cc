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

#include <string>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <openssl/curve25519.h>

#include "../internal.h"


struct SPAKE2Run {
  bool Run() {
    bssl::UniquePtr<SPAKE2_CTX> alice(SPAKE2_CTX_new(
        spake2_role_alice,
        reinterpret_cast<const uint8_t *>(alice_names.first.data()),
        alice_names.first.size(),
        reinterpret_cast<const uint8_t *>(alice_names.second.data()),
        alice_names.second.size()));
    bssl::UniquePtr<SPAKE2_CTX> bob(SPAKE2_CTX_new(
        spake2_role_bob,
        reinterpret_cast<const uint8_t *>(bob_names.first.data()),
        bob_names.first.size(),
        reinterpret_cast<const uint8_t *>(bob_names.second.data()),
        bob_names.second.size()));

    if (!alice || !bob) {
      return false;
    }

    uint8_t alice_msg[SPAKE2_MAX_MSG_SIZE];
    uint8_t bob_msg[SPAKE2_MAX_MSG_SIZE];
    size_t alice_msg_len, bob_msg_len;

    if (!SPAKE2_generate_msg(
            alice.get(), alice_msg, &alice_msg_len, sizeof(alice_msg),
            reinterpret_cast<const uint8_t *>(alice_password.data()),
            alice_password.size()) ||
        !SPAKE2_generate_msg(
            bob.get(), bob_msg, &bob_msg_len, sizeof(bob_msg),
            reinterpret_cast<const uint8_t *>(bob_password.data()),
            bob_password.size())) {
      return false;
    }

    if (alice_corrupt_msg_bit >= 0 &&
        static_cast<size_t>(alice_corrupt_msg_bit) < 8 * alice_msg_len) {
      alice_msg[alice_corrupt_msg_bit/8] ^= 1 << (alice_corrupt_msg_bit & 7);
    }

    uint8_t alice_key[64], bob_key[64];
    size_t alice_key_len, bob_key_len;

    if (!SPAKE2_process_msg(alice.get(), alice_key, &alice_key_len,
                            sizeof(alice_key), bob_msg, bob_msg_len) ||
        !SPAKE2_process_msg(bob.get(), bob_key, &bob_key_len, sizeof(bob_key),
                            alice_msg, alice_msg_len)) {
      return false;
    }

    key_matches_ = (alice_key_len == bob_key_len &&
                    OPENSSL_memcmp(alice_key, bob_key, alice_key_len) == 0);

    return true;
  }

  bool key_matches() const {
    return key_matches_;
  }

  std::string alice_password = "password";
  std::string bob_password = "password";
  std::pair<std::string, std::string> alice_names = {"alice", "bob"};
  std::pair<std::string, std::string> bob_names = {"bob", "alice"};
  int alice_corrupt_msg_bit = -1;

 private:
  bool key_matches_ = false;
};

static bool TestSPAKE2() {
  for (unsigned i = 0; i < 20; i++) {
    SPAKE2Run spake2;
    if (!spake2.Run()) {
      fprintf(stderr, "TestSPAKE2: SPAKE2 failed.\n");
      return false;
    }

    if (!spake2.key_matches()) {
      fprintf(stderr, "Key didn't match for equal passwords.\n");
      return false;
    }
  }

  return true;
}

static bool TestWrongPassword() {
  SPAKE2Run spake2;
  spake2.bob_password = "wrong password";
  if (!spake2.Run()) {
    fprintf(stderr, "TestSPAKE2: SPAKE2 failed.\n");
    return false;
  }

  if (spake2.key_matches()) {
    fprintf(stderr, "Key matched for unequal passwords.\n");
    return false;
  }

  return true;
}

static bool TestWrongNames() {
  SPAKE2Run spake2;
  spake2.alice_names.second = "charlie";
  spake2.bob_names.second = "charlie";
  if (!spake2.Run()) {
    fprintf(stderr, "TestSPAKE2: SPAKE2 failed.\n");
    return false;
  }

  if (spake2.key_matches()) {
    fprintf(stderr, "Key matched for unequal names.\n");
    return false;
  }

  return true;
}

static bool TestCorruptMessages() {
  for (int i = 0; i < 8 * SPAKE2_MAX_MSG_SIZE; i++) {
    SPAKE2Run spake2;
    spake2.alice_corrupt_msg_bit = i;
    if (spake2.Run() && spake2.key_matches()) {
      fprintf(stderr, "Passed after corrupting Alice's message, bit %d\n", i);
      return false;
    }
  }

  return true;
}

/* TODO(agl): add tests with fixed vectors once SPAKE2 is nailed down. */

int main(int argc, char **argv) {
  if (!TestSPAKE2() ||
      !TestWrongPassword() ||
      !TestWrongNames() ||
      !TestCorruptMessages()) {
    return 1;
  }

  printf("PASS\n");
  return 0;
}
