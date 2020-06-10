# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import binascii

import pytest

from cryptography.exceptions import (
    AlreadyFinalized, InvalidKey, _Reasons
)
from cryptography.hazmat.backends.interfaces import HMACBackend
from cryptography.hazmat.backends.interfaces import HashBackend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.concatkdf import ConcatKDFHMAC
from cryptography.hazmat.primitives.kdf.concatkdf import ConcatKDFHash

from ...utils import raises_unsupported_algorithm


@pytest.mark.requires_backend_interface(interface=HashBackend)
class TestConcatKDFHash(object):
    def test_length_limit(self, backend):
        big_length = hashes.SHA256().digest_size * (2 ** 32 - 1) + 1

        with pytest.raises(ValueError):
            ConcatKDFHash(hashes.SHA256(), big_length, None, backend)

    def test_already_finalized(self, backend):
        ckdf = ConcatKDFHash(hashes.SHA256(), 16, None, backend)

        ckdf.derive(b"\x01" * 16)

        with pytest.raises(AlreadyFinalized):
            ckdf.derive(b"\x02" * 16)

    def test_derive(self, backend):
        prk = binascii.unhexlify(
            b"52169af5c485dcc2321eb8d26d5efa21fb9b93c98e38412ee2484cf14f0d0d23"
        )

        okm = binascii.unhexlify(b"1c3bc9e7c4547c5191c0d478cccaed55")

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e53728157e634612c12d6d5223e204aeea4341565369647bd184bcd2"
            b"46f72971f292badaa2fe4124612cba"
        )

        ckdf = ConcatKDFHash(hashes.SHA256(), 16, oinfo, backend)

        assert ckdf.derive(prk) == okm

    def test_verify(self, backend):
        prk = binascii.unhexlify(
            b"52169af5c485dcc2321eb8d26d5efa21fb9b93c98e38412ee2484cf14f0d0d23"
        )

        okm = binascii.unhexlify(b"1c3bc9e7c4547c5191c0d478cccaed55")

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e53728157e634612c12d6d5223e204aeea4341565369647bd184bcd2"
            b"46f72971f292badaa2fe4124612cba"
        )

        ckdf = ConcatKDFHash(hashes.SHA256(), 16, oinfo, backend)

        assert ckdf.verify(prk, okm) is None

    def test_invalid_verify(self, backend):
        prk = binascii.unhexlify(
            b"52169af5c485dcc2321eb8d26d5efa21fb9b93c98e38412ee2484cf14f0d0d23"
        )

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e53728157e634612c12d6d5223e204aeea4341565369647bd184bcd2"
            b"46f72971f292badaa2fe4124612cba"
        )

        ckdf = ConcatKDFHash(hashes.SHA256(), 16, oinfo, backend)

        with pytest.raises(InvalidKey):
            ckdf.verify(prk, b"wrong key")

    def test_unicode_typeerror(self, backend):
        with pytest.raises(TypeError):
            ConcatKDFHash(
                hashes.SHA256(),
                16,
                otherinfo=u"foo",
                backend=backend
            )

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHash(
                hashes.SHA256(),
                16,
                otherinfo=None,
                backend=backend
            )

            ckdf.derive(u"foo")

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHash(
                hashes.SHA256(),
                16,
                otherinfo=None,
                backend=backend
            )

            ckdf.verify(u"foo", b"bar")

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHash(
                hashes.SHA256(),
                16,
                otherinfo=None,
                backend=backend
            )

            ckdf.verify(b"foo", u"bar")


@pytest.mark.requires_backend_interface(interface=HMACBackend)
class TestConcatKDFHMAC(object):
    def test_length_limit(self, backend):
        big_length = hashes.SHA256().digest_size * (2 ** 32 - 1) + 1

        with pytest.raises(ValueError):
            ConcatKDFHMAC(hashes.SHA256(), big_length, None, None, backend)

    def test_already_finalized(self, backend):
        ckdf = ConcatKDFHMAC(hashes.SHA256(), 16, None, None, backend)

        ckdf.derive(b"\x01" * 16)

        with pytest.raises(AlreadyFinalized):
            ckdf.derive(b"\x02" * 16)

    def test_derive(self, backend):
        prk = binascii.unhexlify(
            b"013951627c1dea63ea2d7702dd24e963eef5faac6b4af7e4"
            b"b831cde499dff1ce45f6179f741c728aa733583b02409208"
            b"8f0af7fce1d045edbc5790931e8d5ca79c73"
        )

        okm = binascii.unhexlify(b"64ce901db10d558661f10b6836a122a7"
                                 b"605323ce2f39bf27eaaac8b34cf89f2f")

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e55e600be5f367e0e8a465f4bf2704db00c9325c"
            b"9fbd216d12b49160b2ae5157650f43415653696421e68e"
        )

        ckdf = ConcatKDFHMAC(hashes.SHA512(), 32, None, oinfo, backend)

        assert ckdf.derive(prk) == okm

    def test_derive_explicit_salt(self, backend):
        prk = binascii.unhexlify(
            b"013951627c1dea63ea2d7702dd24e963eef5faac6b4af7e4"
            b"b831cde499dff1ce45f6179f741c728aa733583b02409208"
            b"8f0af7fce1d045edbc5790931e8d5ca79c73"
        )

        okm = binascii.unhexlify(b"64ce901db10d558661f10b6836a122a7"
                                 b"605323ce2f39bf27eaaac8b34cf89f2f")

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e55e600be5f367e0e8a465f4bf2704db00c9325c"
            b"9fbd216d12b49160b2ae5157650f43415653696421e68e"
        )

        ckdf = ConcatKDFHMAC(
            hashes.SHA512(), 32, b"\x00" * 128, oinfo, backend
        )

        assert ckdf.derive(prk) == okm

    def test_verify(self, backend):
        prk = binascii.unhexlify(
            b"013951627c1dea63ea2d7702dd24e963eef5faac6b4af7e4"
            b"b831cde499dff1ce45f6179f741c728aa733583b02409208"
            b"8f0af7fce1d045edbc5790931e8d5ca79c73"
        )

        okm = binascii.unhexlify(b"64ce901db10d558661f10b6836a122a7"
                                 b"605323ce2f39bf27eaaac8b34cf89f2f")

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e55e600be5f367e0e8a465f4bf2704db00c9325c"
            b"9fbd216d12b49160b2ae5157650f43415653696421e68e"
        )

        ckdf = ConcatKDFHMAC(hashes.SHA512(), 32, None, oinfo, backend)

        assert ckdf.verify(prk, okm) is None

    def test_invalid_verify(self, backend):
        prk = binascii.unhexlify(
            b"013951627c1dea63ea2d7702dd24e963eef5faac6b4af7e4"
            b"b831cde499dff1ce45f6179f741c728aa733583b02409208"
            b"8f0af7fce1d045edbc5790931e8d5ca79c73"
        )

        oinfo = binascii.unhexlify(
            b"a1b2c3d4e55e600be5f367e0e8a465f4bf2704db00c9325c"
            b"9fbd216d12b49160b2ae5157650f43415653696421e68e"
        )

        ckdf = ConcatKDFHMAC(hashes.SHA512(), 32, None, oinfo, backend)

        with pytest.raises(InvalidKey):
            ckdf.verify(prk, b"wrong key")

    def test_unicode_typeerror(self, backend):
        with pytest.raises(TypeError):
            ConcatKDFHMAC(
                hashes.SHA256(),
                16, salt=u"foo",
                otherinfo=None,
                backend=backend
            )

        with pytest.raises(TypeError):
            ConcatKDFHMAC(
                hashes.SHA256(),
                16, salt=None,
                otherinfo=u"foo",
                backend=backend
            )

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHMAC(
                hashes.SHA256(),
                16, salt=None,
                otherinfo=None,
                backend=backend
            )

            ckdf.derive(u"foo")

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHMAC(
                hashes.SHA256(),
                16, salt=None,
                otherinfo=None,
                backend=backend
            )

            ckdf.verify(u"foo", b"bar")

        with pytest.raises(TypeError):
            ckdf = ConcatKDFHMAC(
                hashes.SHA256(),
                16, salt=None,
                otherinfo=None,
                backend=backend
            )

            ckdf.verify(b"foo", u"bar")


def test_invalid_backend():
    pretend_backend = object()

    with raises_unsupported_algorithm(_Reasons.BACKEND_MISSING_INTERFACE):
        ConcatKDFHash(hashes.SHA256(), 16, None, pretend_backend)
    with raises_unsupported_algorithm(_Reasons.BACKEND_MISSING_INTERFACE):
        ConcatKDFHMAC(hashes.SHA256(), 16, None, None, pretend_backend)
