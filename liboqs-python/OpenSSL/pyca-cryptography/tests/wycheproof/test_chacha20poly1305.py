# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import binascii

import pytest

from cryptography.exceptions import InvalidTag
from cryptography.hazmat.backends.interfaces import CipherBackend
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305

from ..hazmat.primitives.test_aead import _aead_supported


@pytest.mark.skipif(
    not _aead_supported(ChaCha20Poly1305),
    reason="Requires OpenSSL with ChaCha20Poly1305 support"
)
@pytest.mark.requires_backend_interface(interface=CipherBackend)
@pytest.mark.wycheproof_tests("chacha20_poly1305_test.json")
def test_chacha2poly1305(wycheproof):
    key = binascii.unhexlify(wycheproof.testcase["key"])
    iv = binascii.unhexlify(wycheproof.testcase["iv"])
    aad = binascii.unhexlify(wycheproof.testcase["aad"])
    msg = binascii.unhexlify(wycheproof.testcase["msg"])
    ct = binascii.unhexlify(wycheproof.testcase["ct"])
    tag = binascii.unhexlify(wycheproof.testcase["tag"])

    if wycheproof.valid:
        chacha = ChaCha20Poly1305(key)
        computed_ct = chacha.encrypt(iv, msg, aad)
        assert computed_ct == ct + tag
        computed_msg = chacha.decrypt(iv, ct + tag, aad)
        assert computed_msg == msg
    elif len(iv) != 12:
        chacha = ChaCha20Poly1305(key)
        with pytest.raises(ValueError):
            chacha.encrypt(iv, msg, aad)
        with pytest.raises(ValueError):
            chacha.decrypt(iv, ct + tag, aad)
    else:
        chacha = ChaCha20Poly1305(key)
        with pytest.raises(InvalidTag):
            chacha.decrypt(iv, msg + tag, aad)
