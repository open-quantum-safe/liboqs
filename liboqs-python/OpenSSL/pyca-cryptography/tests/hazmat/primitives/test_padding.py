# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import pytest

import six

from cryptography.exceptions import AlreadyFinalized
from cryptography.hazmat.primitives import padding


class TestPKCS7(object):
    @pytest.mark.parametrize("size", [127, 4096, -2])
    def test_invalid_block_size(self, size):
        with pytest.raises(ValueError):
            padding.PKCS7(size)

    @pytest.mark.parametrize(("size", "padded"), [
        (128, b"1111"),
        (128, b"1111111111111111"),
        (128, b"111111111111111\x06"),
        (128, b""),
        (128, b"\x06" * 6),
        (128, b"\x00" * 16),
    ])
    def test_invalid_padding(self, size, padded):
        unpadder = padding.PKCS7(size).unpadder()
        with pytest.raises(ValueError):
            unpadder.update(padded)
            unpadder.finalize()

    def test_non_bytes(self):
        padder = padding.PKCS7(128).padder()
        with pytest.raises(TypeError):
            padder.update(u"abc")
        unpadder = padding.PKCS7(128).unpadder()
        with pytest.raises(TypeError):
            unpadder.update(u"abc")

    @pytest.mark.parametrize(("size", "unpadded", "padded"), [
        (
            128,
            b"1111111111",
            b"1111111111\x06\x06\x06\x06\x06\x06",
        ),
        (
            128,
            b"111111111111111122222222222222",
            b"111111111111111122222222222222\x02\x02",
        ),
        (
            128,
            b"1" * 16,
            b"1" * 16 + b"\x10" * 16,
        ),
        (
            128,
            b"1" * 17,
            b"1" * 17 + b"\x0F" * 15,
        )
    ])
    def test_pad(self, size, unpadded, padded):
        padder = padding.PKCS7(size).padder()
        result = padder.update(unpadded)
        result += padder.finalize()
        assert result == padded

    @pytest.mark.parametrize(("size", "unpadded", "padded"), [
        (
            128,
            b"1111111111",
            b"1111111111\x06\x06\x06\x06\x06\x06",
        ),
        (
            128,
            b"111111111111111122222222222222",
            b"111111111111111122222222222222\x02\x02",
        ),
    ])
    def test_unpad(self, size, unpadded, padded):
        unpadder = padding.PKCS7(size).unpadder()
        result = unpadder.update(padded)
        result += unpadder.finalize()
        assert result == unpadded

    def test_use_after_finalize(self):
        padder = padding.PKCS7(128).padder()
        b = padder.finalize()
        with pytest.raises(AlreadyFinalized):
            padder.update(b"")
        with pytest.raises(AlreadyFinalized):
            padder.finalize()

        unpadder = padding.PKCS7(128).unpadder()
        unpadder.update(b)
        unpadder.finalize()
        with pytest.raises(AlreadyFinalized):
            unpadder.update(b"")
        with pytest.raises(AlreadyFinalized):
            unpadder.finalize()

    def test_large_padding(self):
        padder = padding.PKCS7(2040).padder()
        padded_data = padder.update(b"")
        padded_data += padder.finalize()

        for i in six.iterbytes(padded_data):
            assert i == 255

        unpadder = padding.PKCS7(2040).unpadder()
        data = unpadder.update(padded_data)
        data += unpadder.finalize()

        assert data == b""


class TestANSIX923(object):
    @pytest.mark.parametrize("size", [127, 4096, -2])
    def test_invalid_block_size(self, size):
        with pytest.raises(ValueError):
            padding.ANSIX923(size)

    @pytest.mark.parametrize(("size", "padded"), [
        (128, b"1111"),
        (128, b"1111111111111111"),
        (128, b"111111111111111\x06"),
        (128, b"1111111111\x06\x06\x06\x06\x06\x06"),
        (128, b""),
        (128, b"\x06" * 6),
        (128, b"\x00" * 16),
    ])
    def test_invalid_padding(self, size, padded):
        unpadder = padding.ANSIX923(size).unpadder()
        with pytest.raises(ValueError):
            unpadder.update(padded)
            unpadder.finalize()

    def test_non_bytes(self):
        padder = padding.ANSIX923(128).padder()
        with pytest.raises(TypeError):
            padder.update(u"abc")
        unpadder = padding.ANSIX923(128).unpadder()
        with pytest.raises(TypeError):
            unpadder.update(u"abc")

    @pytest.mark.parametrize(("size", "unpadded", "padded"), [
        (
            128,
            b"1111111111",
            b"1111111111\x00\x00\x00\x00\x00\x06",
        ),
        (
            128,
            b"111111111111111122222222222222",
            b"111111111111111122222222222222\x00\x02",
        ),
        (
            128,
            b"1" * 16,
            b"1" * 16 + b"\x00" * 15 + b"\x10",
        ),
        (
            128,
            b"1" * 17,
            b"1" * 17 + b"\x00" * 14 + b"\x0F",
        )
    ])
    def test_pad(self, size, unpadded, padded):
        padder = padding.ANSIX923(size).padder()
        result = padder.update(unpadded)
        result += padder.finalize()
        assert result == padded

    @pytest.mark.parametrize(("size", "unpadded", "padded"), [
        (
            128,
            b"1111111111",
            b"1111111111\x00\x00\x00\x00\x00\x06",
        ),
        (
            128,
            b"111111111111111122222222222222",
            b"111111111111111122222222222222\x00\x02",
        ),
    ])
    def test_unpad(self, size, unpadded, padded):
        unpadder = padding.ANSIX923(size).unpadder()
        result = unpadder.update(padded)
        result += unpadder.finalize()
        assert result == unpadded

    def test_use_after_finalize(self):
        padder = padding.ANSIX923(128).padder()
        b = padder.finalize()
        with pytest.raises(AlreadyFinalized):
            padder.update(b"")
        with pytest.raises(AlreadyFinalized):
            padder.finalize()

        unpadder = padding.ANSIX923(128).unpadder()
        unpadder.update(b)
        unpadder.finalize()
        with pytest.raises(AlreadyFinalized):
            unpadder.update(b"")
        with pytest.raises(AlreadyFinalized):
            unpadder.finalize()
