# This file is dual licensed under the terms of the Apache License, Version
# 2.0, and the BSD License. See the LICENSE file in the root of this repository
# for complete details.

from __future__ import absolute_import, division, print_function

import datetime

import pytest

import pytz

from cryptography import x509
from cryptography.hazmat.backends.interfaces import X509Backend


class TestRevokedCertificateBuilder(object):
    def test_serial_number_must_be_integer(self):
        with pytest.raises(TypeError):
            x509.RevokedCertificateBuilder().serial_number("notanx509name")

    def test_serial_number_must_be_non_negative(self):
        with pytest.raises(ValueError):
            x509.RevokedCertificateBuilder().serial_number(-1)

    def test_serial_number_must_be_positive(self):
        with pytest.raises(ValueError):
            x509.RevokedCertificateBuilder().serial_number(0)

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_minimal_serial_number(self, backend):
        revocation_date = datetime.datetime(2002, 1, 1, 12, 1)
        builder = x509.RevokedCertificateBuilder().serial_number(
            1
        ).revocation_date(
            revocation_date
        )

        revoked_certificate = builder.build(backend)
        assert revoked_certificate.serial_number == 1

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_biggest_serial_number(self, backend):
        revocation_date = datetime.datetime(2002, 1, 1, 12, 1)
        builder = x509.RevokedCertificateBuilder().serial_number(
            (1 << 159) - 1
        ).revocation_date(
            revocation_date
        )

        revoked_certificate = builder.build(backend)
        assert revoked_certificate.serial_number == (1 << 159) - 1

    def test_serial_number_must_be_less_than_160_bits_long(self):
        with pytest.raises(ValueError):
            x509.RevokedCertificateBuilder().serial_number(1 << 159)

    def test_set_serial_number_twice(self):
        builder = x509.RevokedCertificateBuilder().serial_number(3)
        with pytest.raises(ValueError):
            builder.serial_number(4)

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_aware_revocation_date(self, backend):
        time = datetime.datetime(2012, 1, 16, 22, 43)
        tz = pytz.timezone("US/Pacific")
        time = tz.localize(time)
        utc_time = datetime.datetime(2012, 1, 17, 6, 43)
        serial_number = 333
        builder = x509.RevokedCertificateBuilder().serial_number(
            serial_number
        ).revocation_date(
            time
        )

        revoked_certificate = builder.build(backend)
        assert revoked_certificate.revocation_date == utc_time

    def test_revocation_date_invalid(self):
        with pytest.raises(TypeError):
            x509.RevokedCertificateBuilder().revocation_date("notadatetime")

    def test_revocation_date_before_unix_epoch(self):
        with pytest.raises(ValueError):
            x509.RevokedCertificateBuilder().revocation_date(
                datetime.datetime(1960, 8, 10)
            )

    def test_set_revocation_date_twice(self):
        builder = x509.RevokedCertificateBuilder().revocation_date(
            datetime.datetime(2002, 1, 1, 12, 1)
        )
        with pytest.raises(ValueError):
            builder.revocation_date(datetime.datetime(2002, 1, 1, 12, 1))

    def test_add_extension_checks_for_duplicates(self):
        builder = x509.RevokedCertificateBuilder().add_extension(
            x509.CRLReason(x509.ReasonFlags.ca_compromise), False
        )

        with pytest.raises(ValueError):
            builder.add_extension(
                x509.CRLReason(x509.ReasonFlags.ca_compromise), False
            )

    def test_add_invalid_extension(self):
        with pytest.raises(TypeError):
            x509.RevokedCertificateBuilder().add_extension(
                "notanextension", False
            )

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_no_serial_number(self, backend):
        builder = x509.RevokedCertificateBuilder().revocation_date(
            datetime.datetime(2002, 1, 1, 12, 1)
        )

        with pytest.raises(ValueError):
            builder.build(backend)

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_no_revocation_date(self, backend):
        builder = x509.RevokedCertificateBuilder().serial_number(3)

        with pytest.raises(ValueError):
            builder.build(backend)

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_create_revoked(self, backend):
        serial_number = 333
        revocation_date = datetime.datetime(2002, 1, 1, 12, 1)
        builder = x509.RevokedCertificateBuilder().serial_number(
            serial_number
        ).revocation_date(
            revocation_date
        )

        revoked_certificate = builder.build(backend)
        assert revoked_certificate.serial_number == serial_number
        assert revoked_certificate.revocation_date == revocation_date
        assert len(revoked_certificate.extensions) == 0

    @pytest.mark.parametrize(
        "extension",
        [
            x509.InvalidityDate(datetime.datetime(2015, 1, 1, 0, 0)),
            x509.CRLReason(x509.ReasonFlags.ca_compromise),
            x509.CertificateIssuer([
                x509.DNSName(u"cryptography.io"),
            ])
        ]
    )
    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_add_extensions(self, backend, extension):
        serial_number = 333
        revocation_date = datetime.datetime(2002, 1, 1, 12, 1)
        builder = x509.RevokedCertificateBuilder().serial_number(
            serial_number
        ).revocation_date(
            revocation_date
        ).add_extension(
            extension, False
        )

        revoked_certificate = builder.build(backend)
        assert revoked_certificate.serial_number == serial_number
        assert revoked_certificate.revocation_date == revocation_date
        assert len(revoked_certificate.extensions) == 1
        ext = revoked_certificate.extensions.get_extension_for_class(
            type(extension)
        )
        assert ext.critical is False
        assert ext.value == extension

    @pytest.mark.requires_backend_interface(interface=X509Backend)
    def test_add_multiple_extensions(self, backend):
        serial_number = 333
        revocation_date = datetime.datetime(2002, 1, 1, 12, 1)
        invalidity_date = x509.InvalidityDate(
            datetime.datetime(2015, 1, 1, 0, 0)
        )
        certificate_issuer = x509.CertificateIssuer([
            x509.DNSName(u"cryptography.io"),
        ])
        crl_reason = x509.CRLReason(x509.ReasonFlags.aa_compromise)
        builder = x509.RevokedCertificateBuilder().serial_number(
            serial_number
        ).revocation_date(
            revocation_date
        ).add_extension(
            invalidity_date, True
        ).add_extension(
            crl_reason, True
        ).add_extension(
            certificate_issuer, True
        )

        revoked_certificate = builder.build(backend)
        assert len(revoked_certificate.extensions) == 3
        for ext_data in [invalidity_date, certificate_issuer, crl_reason]:
            ext = revoked_certificate.extensions.get_extension_for_class(
                type(ext_data)
            )
            assert ext.critical is True
            assert ext.value == ext_data
