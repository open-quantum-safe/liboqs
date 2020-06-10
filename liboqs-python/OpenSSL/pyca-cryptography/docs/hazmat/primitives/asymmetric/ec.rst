.. hazmat::

Elliptic curve cryptography
===========================

.. module:: cryptography.hazmat.primitives.asymmetric.ec


.. function:: generate_private_key(curve, backend)

    .. versionadded:: 0.5

    Generate a new private key on ``curve`` for use with ``backend``.

    :param curve: An instance of :class:`EllipticCurve`.

    :param backend: An instance of
        :class:`~cryptography.hazmat.backends.interfaces.EllipticCurveBackend`.

    :returns: A new instance of :class:`EllipticCurvePrivateKey`.


.. function:: derive_private_key(private_value, curve, backend)

    .. versionadded:: 1.6

    Derive a private key from ``private_value`` on ``curve`` for use with
    ``backend``.

    :param int private_value: The secret scalar value.

    :param curve: An instance of :class:`EllipticCurve`.

    :param backend: An instance of
        :class:`~cryptography.hazmat.backends.interfaces.EllipticCurveBackend`.

    :returns: A new instance of :class:`EllipticCurvePrivateKey`.


Elliptic Curve Signature Algorithms
-----------------------------------

.. class:: ECDSA(algorithm)

    .. versionadded:: 0.5

    The ECDSA signature algorithm first standardized in NIST publication
    `FIPS 186-3`_, and later in `FIPS 186-4`_.

    :param algorithm: An instance of
        :class:`~cryptography.hazmat.primitives.hashes.HashAlgorithm`.

    .. doctest::

        >>> from cryptography.hazmat.backends import default_backend
        >>> from cryptography.hazmat.primitives import hashes
        >>> from cryptography.hazmat.primitives.asymmetric import ec
        >>> private_key = ec.generate_private_key(
        ...     ec.SECP384R1(), default_backend()
        ... )
        >>> data = b"this is some data I'd like to sign"
        >>> signature = private_key.sign(
        ...     data,
        ...     ec.ECDSA(hashes.SHA256())
        ... )

    The ``signature`` is a ``bytes`` object, whose contents is DER encoded as
    described in :rfc:`3279`. This can be decoded using
    :func:`~cryptography.hazmat.primitives.asymmetric.utils.decode_dss_signature`.

    If your data is too large to be passed in a single call, you can hash it
    separately and pass that value using
    :class:`~cryptography.hazmat.primitives.asymmetric.utils.Prehashed`.

    .. doctest::

        >>> from cryptography.hazmat.primitives.asymmetric import utils
        >>> chosen_hash = hashes.SHA256()
        >>> hasher = hashes.Hash(chosen_hash, default_backend())
        >>> hasher.update(b"data & ")
        >>> hasher.update(b"more data")
        >>> digest = hasher.finalize()
        >>> sig = private_key.sign(
        ...     digest,
        ...     ec.ECDSA(utils.Prehashed(chosen_hash))
        ... )


    Verification requires the public key, the signature itself, the signed
    data, and knowledge of the hashing algorithm that was used when producing
    the signature:

    >>> public_key = private_key.public_key()
    >>> public_key.verify(signature, data, ec.ECDSA(hashes.SHA256()))

    If the signature is not valid, an
    :class:`~cryptography.exceptions.InvalidSignature` exception will be raised.

    If your data is too large to be passed in a single call, you can hash it
    separately and pass that value using
    :class:`~cryptography.hazmat.primitives.asymmetric.utils.Prehashed`.

    .. doctest::

        >>> chosen_hash = hashes.SHA256()
        >>> hasher = hashes.Hash(chosen_hash, default_backend())
        >>> hasher.update(b"data & ")
        >>> hasher.update(b"more data")
        >>> digest = hasher.finalize()
        >>> public_key.verify(
        ...     sig,
        ...     digest,
        ...     ec.ECDSA(utils.Prehashed(chosen_hash))
        ... )

    .. note::
        Although in this case the public key was derived from the private one,
        in a typical setting you will not possess the private key. The
        `Key loading`_ section explains how to load the public key from other
        sources.


.. class:: EllipticCurvePrivateNumbers(private_value, public_numbers)

    .. versionadded:: 0.5

    The collection of integers that make up an EC private key.

    .. attribute:: public_numbers

        :type: :class:`~cryptography.hazmat.primitives.asymmetric.ec.EllipticCurvePublicNumbers`

        The :class:`EllipticCurvePublicNumbers` which makes up the EC public
        key associated with this EC private key.

    .. attribute:: private_value

        :type: int

        The private value.

    .. method:: private_key(backend)

        Convert a collection of numbers into a private key suitable for doing
        actual cryptographic operations.

        :param backend: An instance of
            :class:`~cryptography.hazmat.backends.interfaces.EllipticCurveBackend`.

        :returns: A new instance of :class:`EllipticCurvePrivateKey`.


.. class:: EllipticCurvePublicNumbers(x, y, curve)

    .. warning::
        The point represented by this object is not validated in any way until
        :meth:`EllipticCurvePublicNumbers.public_key` is called and may not
        represent a valid point on the curve. You should not attempt to perform
        any computations using the values from this class until you have either
        validated it yourself or called ``public_key()`` successfully.

    .. versionadded:: 0.5

    The collection of integers that make up an EC public key.

     .. attribute:: curve

        :type: :class:`EllipticCurve`

        The elliptic curve for this key.

    .. attribute:: x

        :type: int

        The affine x component of the public point used for verifying.

    .. attribute:: y

        :type: int

        The affine y component of the public point used for verifying.

    .. method:: public_key(backend)

        Convert a collection of numbers into a public key suitable for doing
        actual cryptographic operations.

        :param backend: An instance of
            :class:`~cryptography.hazmat.backends.interfaces.EllipticCurveBackend`.

        :raises ValueError: Raised if the point is invalid for the curve.
        :returns: A new instance of :class:`EllipticCurvePublicKey`.

    .. method:: encode_point()

        .. versionadded:: 1.1

        Encodes an elliptic curve point to a byte string as described in
        `SEC 1 v2.0`_ section 2.3.3. This method only supports uncompressed
        points.

        :return bytes: The encoded point.

    .. classmethod:: from_encoded_point(curve, data)

        .. versionadded:: 1.1

        Decodes a byte string as described in `SEC 1 v2.0`_ section 2.3.3 and
        returns an :class:`EllipticCurvePublicNumbers`. This method only
        supports uncompressed points.

        :param curve: An
            :class:`~cryptography.hazmat.primitives.asymmetric.ec.EllipticCurve`
            instance.

        :param bytes data: The serialized point byte string.

        :returns: An :class:`EllipticCurvePublicNumbers` instance.

        :raises ValueError: Raised on invalid point type or data length.

        :raises TypeError: Raised when curve is not an
            :class:`~cryptography.hazmat.primitives.asymmetric.ec.EllipticCurve`.

Elliptic Curve Key Exchange algorithm
-------------------------------------

.. class:: ECDH()

    .. versionadded:: 1.1

    The Elliptic Curve Diffie-Hellman Key Exchange algorithm first standardized
    in NIST publication `800-56A`_, and later in `800-56Ar2`_.

    For most applications the ``shared_key`` should be passed to a key
    derivation function. This allows mixing of additional information into the
    key, derivation of multiple keys, and destroys any structure that may be
    present.

    .. doctest::

        >>> from cryptography.hazmat.backends import default_backend
        >>> from cryptography.hazmat.primitives import hashes
        >>> from cryptography.hazmat.primitives.asymmetric import ec
        >>> from cryptography.hazmat.primitives.kdf.hkdf import HKDF
        >>> # Generate a private key for use in the exchange.
        >>> private_key = ec.generate_private_key(
        ...     ec.SECP384R1(), default_backend()
        ... )
        >>> # In a real handshake the peer_public_key will be received from the
        >>> # other party. For this example we'll generate another private key
        >>> # and get a public key from that.
        >>> peer_public_key = ec.generate_private_key(
        ...     ec.SECP384R1(), default_backend()
        ... ).public_key()
        >>> shared_key = private_key.exchange(ec.ECDH(), peer_public_key)
        >>> # Perform key derivation.
        >>> derived_key = HKDF(
        ...     algorithm=hashes.SHA256(),
        ...     length=32,
        ...     salt=None,
        ...     info=b'handshake data',
        ...     backend=default_backend()
        ... ).derive(shared_key)
        >>> # For the next handshake we MUST generate another private key.
        >>> private_key_2 = ec.generate_private_key(
        ...     ec.SECP384R1(), default_backend()
        ... )
        >>> peer_public_key_2 = ec.generate_private_key(
        ...     ec.SECP384R1(), default_backend()
        ... ).public_key()
        >>> shared_key_2 = private_key_2.exchange(ec.ECDH(), peer_public_key_2)
        >>> derived_key_2 = HKDF(
        ...     algorithm=hashes.SHA256(),
        ...     length=32,
        ...     salt=None,
        ...     info=b'handshake data',
        ...     backend=default_backend()
        ... ).derive(shared_key_2)

    ECDHE (or EECDH), the ephemeral form of this exchange, is **strongly
    preferred** over simple ECDH and provides `forward secrecy`_ when used.
    You must generate a new private key using :func:`generate_private_key` for
    each :meth:`~EllipticCurvePrivateKey.exchange` when performing an ECDHE key
    exchange.

Elliptic Curves
---------------

Elliptic curves provide equivalent security at much smaller key sizes than
other asymmetric cryptography systems such as RSA or DSA. For many operations
elliptic curves are also significantly faster; `elliptic curve diffie-hellman
is faster than diffie-hellman`_.

.. note::
    Curves with a size of `less than 224 bits`_ should not be used. You should
    strongly consider using curves of at least 224 :term:`bits`.

Generally the NIST prime field ("P") curves are significantly faster than the
other types suggested by NIST at both signing and verifying with ECDSA.

Prime fields also `minimize the number of security concerns for elliptic-curve
cryptography`_. However, there is `some concern`_ that both the prime field and
binary field ("B") NIST curves may have been weakened during their generation.

Currently `cryptography` only supports NIST curves, none of which are
considered "safe" by the `SafeCurves`_ project run by Daniel J. Bernstein and
Tanja Lange.

All named curves are instances of :class:`EllipticCurve`.

.. class:: SECT571K1

    .. versionadded:: 0.5

    SECG curve ``sect571k1``. Also called NIST K-571.


.. class:: SECT409K1

    .. versionadded:: 0.5

    SECG curve ``sect409k1``. Also called NIST K-409.


.. class:: SECT283K1

    .. versionadded:: 0.5

    SECG curve ``sect283k1``. Also called NIST K-283.


.. class:: SECT233K1

    .. versionadded:: 0.5

    SECG curve ``sect233k1``. Also called NIST K-233.


.. class:: SECT163K1

    .. versionadded:: 0.5

    SECG curve ``sect163k1``. Also called NIST K-163.


.. class:: SECT571R1

    .. versionadded:: 0.5

    SECG curve ``sect571r1``. Also called NIST B-571.


.. class:: SECT409R1

    .. versionadded:: 0.5

    SECG curve ``sect409r1``. Also called NIST B-409.


.. class:: SECT283R1

    .. versionadded:: 0.5

    SECG curve ``sect283r1``. Also called NIST B-283.


.. class:: SECT233R1

    .. versionadded:: 0.5

    SECG curve ``sect233r1``. Also called NIST B-233.


.. class:: SECT163R2

    .. versionadded:: 0.5

    SECG curve ``sect163r2``. Also called NIST B-163.


.. class:: SECP521R1

    .. versionadded:: 0.5

    SECG curve ``secp521r1``. Also called NIST P-521.


.. class:: SECP384R1

    .. versionadded:: 0.5

    SECG curve ``secp384r1``. Also called NIST P-384.


.. class:: SECP256R1

    .. versionadded:: 0.5

    SECG curve ``secp256r1``. Also called NIST P-256.


.. class:: SECP224R1

    .. versionadded:: 0.5

    SECG curve ``secp224r1``. Also called NIST P-224.


.. class:: SECP192R1

    .. versionadded:: 0.5

    SECG curve ``secp192r1``. Also called NIST P-192.


.. class:: SECP256K1

    .. versionadded:: 0.9

    SECG curve ``secp256k1``.


.. class:: BrainpoolP256R1

    .. versionadded:: 2.2

    Brainpool curve specified in :rfc:`5639`. These curves are discouraged
    for new systems.

.. class:: BrainpoolP384R1

    .. versionadded:: 2.2

    Brainpool curve specified in :rfc:`5639`. These curves are discouraged
    for new systems.

.. class:: BrainpoolP512R1

    .. versionadded:: 2.2

    Brainpool curve specified in :rfc:`5639`. These curves are discouraged
    for new systems.


Key Interfaces
~~~~~~~~~~~~~~

.. class:: EllipticCurve

    .. versionadded:: 0.5

    A named elliptic curve.

    .. attribute:: name

        :type: string

        The name of the curve. Usually the name used for the ASN.1 OID such as
        ``secp256k1``.

    .. attribute:: key_size

        :type: int

        Size (in :term:`bits`) of a secret scalar for the curve (as generated
        by :func:`generate_private_key`).


.. class:: EllipticCurveSignatureAlgorithm

    .. versionadded:: 0.5
    .. versionchanged:: 1.6
        :class:`~cryptography.hazmat.primitives.asymmetric.utils.Prehashed`
        can now be used as an ``algorithm``.

    A signature algorithm for use with elliptic curve keys.

    .. attribute:: algorithm

        :type: :class:`~cryptography.hazmat.primitives.hashes.HashAlgorithm` or
            :class:`~cryptography.hazmat.primitives.asymmetric.utils.Prehashed`

        The digest algorithm to be used with the signature scheme.


.. class:: EllipticCurvePrivateKey

    .. versionadded:: 0.5

    An elliptic curve private key for use with an algorithm such as `ECDSA`_ or
    `EdDSA`_. An elliptic curve private key that is not an
    :term:`opaque key` also implements
    :class:`EllipticCurvePrivateKeyWithSerialization` to provide serialization
    methods.

    .. method:: exchange(algorithm, peer_public_key)

        .. versionadded:: 1.1

        Performs a key exchange operation using the provided algorithm with
        the peer's public key.

        For most applications the ``shared_key`` should be passed to a key
        derivation function. This allows mixing of additional information into the
        key, derivation of multiple keys, and destroys any structure that may be
        present.

        :param algorithm: The key exchange algorithm, currently only
            :class:`~cryptography.hazmat.primitives.asymmetric.ec.ECDH` is
            supported.
        :param EllipticCurvePublicKey peer_public_key: The public key for the
            peer.

        :returns bytes: A shared key.

    .. method:: public_key()

        :return: :class:`EllipticCurvePublicKey`

        The EllipticCurvePublicKey object for this private key.

    .. method:: sign(data, signature_algorithm)

        .. versionadded:: 1.5

        Sign one block of data which can be verified later by others using the
        public key.

        :param bytes data: The message string to sign.

        :param signature_algorithm: An instance of
            :class:`EllipticCurveSignatureAlgorithm`, such as :class:`ECDSA`.

        :return bytes: Signature.

    .. attribute:: key_size

        .. versionadded:: 1.9

        :type: int

        Size (in :term:`bits`) of a secret scalar for the curve (as generated
        by :func:`generate_private_key`).


.. class:: EllipticCurvePrivateKeyWithSerialization

    .. versionadded:: 0.8

    This interface contains additional methods relating to serialization.
    Any object with this interface also has all the methods from
    :class:`EllipticCurvePrivateKey`.

    .. method:: private_numbers()

        Create a :class:`EllipticCurvePrivateNumbers` object.

        :returns: An :class:`EllipticCurvePrivateNumbers` instance.

    .. method:: private_bytes(encoding, format, encryption_algorithm)

        Allows serialization of the key to bytes. Encoding (
        :attr:`~cryptography.hazmat.primitives.serialization.Encoding.PEM` or
        :attr:`~cryptography.hazmat.primitives.serialization.Encoding.DER`),
        format (
        :attr:`~cryptography.hazmat.primitives.serialization.PrivateFormat.TraditionalOpenSSL`
        or
        :attr:`~cryptography.hazmat.primitives.serialization.PrivateFormat.PKCS8`)
        and encryption algorithm (such as
        :class:`~cryptography.hazmat.primitives.serialization.BestAvailableEncryption`
        or :class:`~cryptography.hazmat.primitives.serialization.NoEncryption`)
        are chosen to define the exact serialization.

        :param encoding: A value from the
            :class:`~cryptography.hazmat.primitives.serialization.Encoding` enum.

        :param format: A value from the
            :class:`~cryptography.hazmat.primitives.serialization.PrivateFormat` enum.

        :param encryption_algorithm: An instance of an object conforming to the
            :class:`~cryptography.hazmat.primitives.serialization.KeySerializationEncryption`
            interface.

        :return bytes: Serialized key.


.. class:: EllipticCurvePublicKey

    .. versionadded:: 0.5

    An elliptic curve public key.

     .. attribute:: curve

        :type: :class:`EllipticCurve`

        The elliptic curve for this key.

    .. method:: public_numbers()

        Create a :class:`EllipticCurvePublicNumbers` object.

        :returns: An :class:`EllipticCurvePublicNumbers` instance.

    .. method:: public_bytes(encoding, format)

        Allows serialization of the key to bytes. Encoding (
        :attr:`~cryptography.hazmat.primitives.serialization.Encoding.PEM` or
        :attr:`~cryptography.hazmat.primitives.serialization.Encoding.DER`) and
        format (
        :attr:`~cryptography.hazmat.primitives.serialization.PublicFormat.SubjectPublicKeyInfo`)
        are chosen to define the exact serialization.

        :param encoding: A value from the
            :class:`~cryptography.hazmat.primitives.serialization.Encoding` enum.

        :param format: A value from the
            :class:`~cryptography.hazmat.primitives.serialization.PublicFormat` enum.

        :return bytes: Serialized key.

    .. method:: verify(signature, data, signature_algorithm)

        .. versionadded:: 1.5

        Verify one block of data was signed by the private key associated
        with this public key.

        :param bytes signature: The signature to verify.

        :param bytes data: The message string that was signed.

        :param signature_algorithm: An instance of
            :class:`EllipticCurveSignatureAlgorithm`.

        :raises cryptography.exceptions.InvalidSignature: If the signature does
            not validate.

    .. attribute:: key_size

        .. versionadded:: 1.9

        :type: int

        Size (in :term:`bits`) of a secret scalar for the curve (as generated
        by :func:`generate_private_key`).


.. class:: EllipticCurvePublicKeyWithSerialization

    .. versionadded:: 0.6

    Alias for :class:`EllipticCurvePublicKey`.



Serialization
~~~~~~~~~~~~~

This sample demonstrates how to generate a private key and serialize it.


.. doctest::

    >>> from cryptography.hazmat.backends import default_backend
    >>> from cryptography.hazmat.primitives import hashes
    >>> from cryptography.hazmat.primitives.asymmetric import ec
    >>> from cryptography.hazmat.primitives import serialization

    >>> private_key = ec.generate_private_key(ec.SECP384R1(), default_backend())

    >>> serialized_private = private_key.private_bytes(
    ...     encoding=serialization.Encoding.PEM,
    ...     format=serialization.PrivateFormat.PKCS8,
    ...     encryption_algorithm=serialization.BestAvailableEncryption(b'testpassword')
    ... )
    >>> serialized_private.splitlines()[0]
    b'-----BEGIN ENCRYPTED PRIVATE KEY-----'

You can also serialize the key without a password, by relying on
:class:`~cryptography.hazmat.primitives.serialization.NoEncryption`.

The public key is serialized as follows:


.. doctest::

    >>> public_key = private_key.public_key()
    >>> serialized_public = public_key.public_bytes(
    ...     encoding=serialization.Encoding.PEM,
    ...     format=serialization.PublicFormat.SubjectPublicKeyInfo
    ... )
    >>> serialized_public.splitlines()[0]
    b'-----BEGIN PUBLIC KEY-----'

This is the part that you would normally share with the rest of the world.


Key loading
~~~~~~~~~~~

This extends the sample in the previous section, assuming that the variables
``serialized_private`` and ``serialized_public`` contain the respective keys
in PEM format.

.. doctest::

    >>> loaded_public_key = serialization.load_pem_public_key(
    ...     serialized_public,
    ...     backend=default_backend()
    ... )

    >>> loaded_private_key = serialization.load_pem_private_key(
    ...     serialized_private,
    ...     # or password=None, if in plain text
    ...     password=b'testpassword',
    ...     backend=default_backend()
    ... )


.. _`FIPS 186-3`: https://csrc.nist.gov/csrc/media/publications/fips/186/3/archive/2009-06-25/documents/fips_186-3.pdf
.. _`FIPS 186-4`: https://csrc.nist.gov/publications/detail/fips/186/4/final
.. _`800-56A`: https://csrc.nist.gov/publications/detail/sp/800-56a/revised/archive/2007-03-14
.. _`800-56Ar2`: https://csrc.nist.gov/publications/detail/sp/800-56a/rev-2/final
.. _`some concern`: https://crypto.stackexchange.com/questions/10263/should-we-trust-the-nist-recommended-ecc-parameters
.. _`less than 224 bits`: http://www.ecrypt.eu.org/ecrypt2/documents/D.SPA.20.pdf
.. _`elliptic curve diffie-hellman is faster than diffie-hellman`: http://digitalcommons.unl.edu/cgi/viewcontent.cgi?article=1100&context=cseconfwork
.. _`minimize the number of security concerns for elliptic-curve cryptography`: https://cr.yp.to/ecdh/curve25519-20060209.pdf
.. _`SafeCurves`: https://safecurves.cr.yp.to/
.. _`ECDSA`: https://en.wikipedia.org/wiki/ECDSA
.. _`EdDSA`: https://en.wikipedia.org/wiki/EdDSA
.. _`forward secrecy`: https://en.wikipedia.org/wiki/Forward_secrecy
.. _`SEC 1 v2.0`: http://www.secg.org/sec1-v2.pdf
