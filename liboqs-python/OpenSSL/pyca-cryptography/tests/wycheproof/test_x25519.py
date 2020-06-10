# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import binascii

import pytest

from cryptography.hazmat.backends.interfaces import DHBackend
from cryptography.hazmat.primitives.asymmetric.x25519 import (
    X25519PrivateKey, X25519PublicKey
)


@pytest.mark.supported(
    only_if=lambda backend: backend.x25519_supported(),
    skip_message="Requires OpenSSL with X25519 support"
)
@pytest.mark.requires_backend_interface(interface=DHBackend)
@pytest.mark.wycheproof_tests("x25519_test.json")
def test_x25519(backend, wycheproof):
    assert list(wycheproof.testgroup.items()) == [("curve", "curve25519")]

    private_key = X25519PrivateKey._from_private_bytes(
        binascii.unhexlify(wycheproof.testcase["private"])
    )
    public_key = X25519PublicKey.from_public_bytes(
        binascii.unhexlify(wycheproof.testcase["public"])
    )

    assert wycheproof.valid or wycheproof.acceptable

    expected = binascii.unhexlify(wycheproof.testcase["shared"])
    if expected == b"\x00" * 32:
        assert wycheproof.acceptable
        # OpenSSL returns an error on all zeros shared key
        with pytest.raises(ValueError):
            private_key.exchange(public_key)
    else:
        assert private_key.exchange(public_key) == expected
