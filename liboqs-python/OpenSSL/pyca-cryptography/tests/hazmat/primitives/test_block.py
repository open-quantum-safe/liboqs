# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import binascii

import pytest

from cryptography.exceptions import (
    AlreadyFinalized, _Reasons
)
from cryptography.hazmat.backends.interfaces import CipherBackend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, base, modes
)

from .utils import (
    generate_aead_exception_test, generate_aead_tag_exception_test
)
from ...doubles import DummyCipherAlgorithm, DummyMode
from ...utils import raises_unsupported_algorithm


@pytest.mark.requires_backend_interface(interface=CipherBackend)
class TestCipher(object):
    def test_creates_encryptor(self, backend):
        cipher = Cipher(
            algorithms.AES(binascii.unhexlify(b"0" * 32)),
            modes.CBC(binascii.unhexlify(b"0" * 32)),
            backend
        )
        assert isinstance(cipher.encryptor(), base.CipherContext)

    def test_creates_decryptor(self, backend):
        cipher = Cipher(
            algorithms.AES(binascii.unhexlify(b"0" * 32)),
            modes.CBC(binascii.unhexlify(b"0" * 32)),
            backend
        )
        assert isinstance(cipher.decryptor(), base.CipherContext)

    def test_instantiate_with_non_algorithm(self, backend):
        algorithm = object()
        with pytest.raises(TypeError):
            Cipher(algorithm, mode=None, backend=backend)


@pytest.mark.requires_backend_interface(interface=CipherBackend)
class TestCipherContext(object):
    def test_use_after_finalize(self, backend):
        cipher = Cipher(
            algorithms.AES(binascii.unhexlify(b"0" * 32)),
            modes.CBC(binascii.unhexlify(b"0" * 32)),
            backend
        )
        encryptor = cipher.encryptor()
        encryptor.update(b"a" * 16)
        encryptor.finalize()
        with pytest.raises(AlreadyFinalized):
            encryptor.update(b"b" * 16)
        with pytest.raises(AlreadyFinalized):
            encryptor.finalize()
        decryptor = cipher.decryptor()
        decryptor.update(b"a" * 16)
        decryptor.finalize()
        with pytest.raises(AlreadyFinalized):
            decryptor.update(b"b" * 16)
        with pytest.raises(AlreadyFinalized):
            decryptor.finalize()

    def test_use_update_into_after_finalize(self, backend):
        cipher = Cipher(
            algorithms.AES(binascii.unhexlify(b"0" * 32)),
            modes.CBC(binascii.unhexlify(b"0" * 32)),
            backend
        )
        encryptor = cipher.encryptor()
        encryptor.update(b"a" * 16)
        encryptor.finalize()
        with pytest.raises(AlreadyFinalized):
            buf = bytearray(31)
            encryptor.update_into(b"b" * 16, buf)

    def test_unaligned_block_encryption(self, backend):
        cipher = Cipher(
            algorithms.AES(binascii.unhexlify(b"0" * 32)),
            modes.ECB(),
            backend
        )
        encryptor = cipher.encryptor()
        ct = encryptor.update(b"a" * 15)
        assert ct == b""
        ct += encryptor.update(b"a" * 65)
        assert len(ct) == 80
        ct += encryptor.finalize()
        decryptor = cipher.decryptor()
        pt = decryptor.update(ct[:3])
        assert pt == b""
        pt += decryptor.update(ct[3:])
        assert len(pt) == 80
        assert pt == b"a" * 80
        decryptor.finalize()

    @pytest.mark.parametrize("mode", [DummyMode(), None])
    def test_nonexistent_cipher(self, backend, mode):
        cipher = Cipher(
            DummyCipherAlgorithm(), mode, backend
        )
        with raises_unsupported_algorithm(_Reasons.UNSUPPORTED_CIPHER):
            cipher.encryptor()

        with raises_unsupported_algorithm(_Reasons.UNSUPPORTED_CIPHER):
            cipher.decryptor()

    def test_incorrectly_padded(self, backend):
        cipher = Cipher(
            algorithms.AES(b"\x00" * 16),
            modes.CBC(b"\x00" * 16),
            backend
        )
        encryptor = cipher.encryptor()
        encryptor.update(b"1")
        with pytest.raises(ValueError):
            encryptor.finalize()

        decryptor = cipher.decryptor()
        decryptor.update(b"1")
        with pytest.raises(ValueError):
            decryptor.finalize()


@pytest.mark.supported(
    only_if=lambda backend: backend.cipher_supported(
        algorithms.AES(b"\x00" * 16), modes.GCM(b"\x00" * 12)
    ),
    skip_message="Does not support AES GCM",
)
@pytest.mark.requires_backend_interface(interface=CipherBackend)
class TestAEADCipherContext(object):
    test_aead_exceptions = generate_aead_exception_test(
        algorithms.AES,
        modes.GCM,
    )
    test_aead_tag_exceptions = generate_aead_tag_exception_test(
        algorithms.AES,
        modes.GCM,
    )


@pytest.mark.requires_backend_interface(interface=CipherBackend)
class TestModeValidation(object):
    def test_cbc(self, backend):
        with pytest.raises(ValueError):
            Cipher(
                algorithms.AES(b"\x00" * 16),
                modes.CBC(b"abc"),
                backend,
            )

    def test_ofb(self, backend):
        with pytest.raises(ValueError):
            Cipher(
                algorithms.AES(b"\x00" * 16),
                modes.OFB(b"abc"),
                backend,
            )

    def test_cfb(self, backend):
        with pytest.raises(ValueError):
            Cipher(
                algorithms.AES(b"\x00" * 16),
                modes.CFB(b"abc"),
                backend,
            )

    def test_cfb8(self, backend):
        with pytest.raises(ValueError):
            Cipher(
                algorithms.AES(b"\x00" * 16),
                modes.CFB8(b"abc"),
                backend,
            )

    def test_ctr(self, backend):
        with pytest.raises(ValueError):
            Cipher(
                algorithms.AES(b"\x00" * 16),
                modes.CTR(b"abc"),
                backend,
            )

    def test_gcm(self):
        with pytest.raises(ValueError):
            modes.GCM(b"")


class TestModesRequireBytes(object):
    def test_cbc(self):
        with pytest.raises(TypeError):
            modes.CBC([1] * 16)

    def test_cfb(self):
        with pytest.raises(TypeError):
            modes.CFB([1] * 16)

    def test_cfb8(self):
        with pytest.raises(TypeError):
            modes.CFB8([1] * 16)

    def test_ofb(self):
        with pytest.raises(TypeError):
            modes.OFB([1] * 16)

    def test_ctr(self):
        with pytest.raises(TypeError):
            modes.CTR([1] * 16)

    def test_gcm_iv(self):
        with pytest.raises(TypeError):
            modes.GCM([1] * 16)

    def test_gcm_tag(self):
        with pytest.raises(TypeError):
            modes.GCM(b"\x00" * 16, [1] * 16)
