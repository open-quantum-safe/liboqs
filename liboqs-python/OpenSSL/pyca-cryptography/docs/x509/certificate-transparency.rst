Certificate Transparency
========================

.. currentmodule:: cryptography.x509.certificate_transparency

`Certificate Transparency`_ is a set of protocols specified in :rfc:`6962`
which allow X.509 certificates to be sent to append-only logs and have small
cryptographic proofs that a certificate has been publicly logged. This allows
for external auditing of the certificates that a certificate authority has
issued.

.. class:: SignedCertificateTimestamp

    .. versionadded:: 2.0

    SignedCertificateTimestamps (SCTs) are small cryptographically signed
    assertions that the specified certificate has been submitted to a
    Certificate Transparency Log, and that it will be part of the public log
    within some time period, this is called the "maximum merge delay" (MMD) and
    each log specifies its own.

    .. attribute:: version

        :type: :class:`~cryptography.x509.certificate_transparency.Version`

        The SCT version as an enumeration. Currently only one version has been
        specified.

    .. attribute:: log_id

        :type: bytes

        An opaque identifier, indicating which log this SCT is from. This is
        the SHA256 hash of the log's public key.

    .. attribute:: timestamp

        :type: :class:`datetime.datetime`

        A naïve datetime representing the time in UTC at which the log asserts
        the certificate had been submitted to it.

    .. attribute:: entry_type

        :type:
            :class:`~cryptography.x509.certificate_transparency.LogEntryType`

        The type of submission to the log that this SCT is for. Log submissions
        can either be certificates themselves or "pre-certificates" which
        indicate a binding-intent to issue a certificate for the same data,
        with SCTs embedded in it.


.. class:: Version

    .. versionadded:: 2.0

    An enumeration for SignedCertificateTimestamp versions.

    .. attribute:: v1

        For version 1 SignedCertificateTimestamps.

.. class:: LogEntryType

    .. versionadded:: 2.0

    An enumeration for SignedCertificateTimestamp log entry types.

    .. attribute:: X509_CERTIFICATE

        For SCTs corresponding to X.509 certificates.

    .. attribute:: PRE_CERTIFICATE

        For SCTs corresponding to pre-certificates.


.. _`Certificate Transparency`: https://www.certificate-transparency.org/
