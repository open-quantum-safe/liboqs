#!/usr/bin/env python3

# TODO: support remote config

import abc
from enum import Enum, EnumType
import logging
import os
import sys
from typing import Any, Mapping
import warnings

import yaml

logger = logging.getLogger(__name__)
handler = logging.StreamHandler(sys.stdout)
handler.setFormatter(logging.Formatter("%(asctime)s %(levelname)s: %(message)s"))
logger.setLevel(logging.INFO)
logger.addHandler(handler)

_liboqs_dir = os.getenv("LIBOQS_DIR")
if not _liboqs_dir:
    _liboqs_dir = os.getcwd()
    warnings.warn("LIBOQS_DIR not set; defaulting to cwd")
LIBOQS_DIR = _liboqs_dir


class OQSBuilderConfig:
    def __init__(self, oqs_meta_path: str | None = None):
        self.oqs_meta_path = oqs_meta_path or os.path.join(LIBOQS_DIR, "OQS_META.yml")
        self.demo_algfamilies = ["demo_alg"]


class FieldValidator(abc.ABC):
    # FIX: this method should return (ok, errmsg_or_none)
    @abc.abstractmethod
    def validate(self, value: Any) -> bool:
        """Return true if value satisfies all requirements of a field"""
        raise NotImplementedError()


class Fields:
    REQUIRED = True
    OPTIONAL = False

    class Boolean(FieldValidator):
        def validate(self, value: Any) -> bool:
            return isinstance(value, bool)

    class Integer(FieldValidator):
        def __init__(self, nonneg: bool = True):
            self.nonneg = nonneg

        def validate(self, value: Any) -> bool:
            if not isinstance(value, int):
                return False
            if self.nonneg:
                return value >= 0
            return True

    class Text(FieldValidator):
        def __init__(self, allowempty: bool = False):
            self.allowempty = allowempty

        def validate(self, value: Any) -> bool:
            if not isinstance(value, str):
                return False
            if not self.allowempty:
                return len(value) > 0
            return True

    class Enumerated(FieldValidator):
        def __init__(self, enum: EnumType):
            self.enum = enum

        def validate(self, value: Any) -> bool:
            return value in self.enum

    class Array(FieldValidator):
        def __init__(self, validator: FieldValidator, allowempty: bool = True):
            self.validator = validator
            self.allowempty = allowempty

        def validate(self, value: Any) -> bool:
            if not isinstance(value, list):
                # This is only used to validate YAML datasheet. PyYAML will
                # always parse array into Python list
                return False
            if (not self.allowempty) and (len(value) == 0):
                return False
            return all([self.validator.validate(item) for item in value])

    class KeyedArray(FieldValidator):
        def __init__(self, validator: FieldValidator):
            self.validator = validator

        def validate(self, value: Any) -> bool:
            if not isinstance(value, dict):
                return False
            return all([self.validator.validate(data) for data in value.values()])

    class Mapping(FieldValidator):
        def __init__(
            self,
            validators: Mapping[str, tuple[bool, FieldValidator]],
            allowextrakeys: bool = False,
        ):
            self.validators = validators
            self.allowextrakeys = allowextrakeys

        def validate(self, value: Any) -> bool:
            if not isinstance(value, dict):
                return False
            for key in value.keys():
                if (not self.allowextrakeys) and (key not in self.validators.keys()):
                    return False
            for key, (required, dator) in self.validators.items():
                subtarget = value.get(key, None)
                if (subtarget is not None) and (not dator.validate(subtarget)):
                    logger.error(f"failed to validate {key}")
                    return False
                if (subtarget is None) and required:
                    logger.error(f"missing key {key}")
                    return False
            return True

    class Union(FieldValidator):
        def __init__(self, validators: set[FieldValidator]):
            self.validators = validators

        def validate(self, value: Any) -> bool:
            return any([dator.validate(value) for dator in self.validators])


class AlgTypes(Enum):
    Kem = "kem"
    Sig = "sig"
    StflSig = "stfl_sig"


class OQSSupportTiers(Enum):
    Core = 1
    Supported = 2
    Community = 3


class UArch(Enum):
    Portable = "portable"
    X86_64 = "x86_64"
    ArmV8 = "arm_8"
    Cuda = "cuda"
    Icicle = "icicle"


class SupportedOS(Enum):
    Linux = "Linux"
    Darwin = "Darwin"


class CpuFeatures(Enum):
    AVX2 = "avx2"
    BMI2 = "bmi2"
    POPCNT = "popcnt"
    NEON = "asimd"


class CmakeInterfaces(Enum):
    Public = "public"
    Private = "private"
    Interface = "interface"
    Target = "target"


class SecStrength(Enum):
    Cat1 = 1
    Cat2 = 2
    Cat3 = 3
    Cat4 = 4
    Cat5 = 5


UPSTREAM_DATOR = Fields.Mapping(
    {
        "git_url": (Fields.REQUIRED, Fields.Text()),
        "git_commit": (Fields.REQUIRED, Fields.Text()),
        "git_branch": (Fields.OPTIONAL, Fields.Text()),
        "patches": (Fields.OPTIONAL, Fields.Array(Fields.Text())),
        "jasminc": (Fields.OPTIONAL, Fields.Text()),
        "post_patches": (Fields.OPTIONAL, Fields.Text()),
    }
)

KEM_PARAM_DATOR = Fields.Mapping(
    {
        "security-strength": (Fields.REQUIRED, Fields.Enumerated(SecStrength)),
        "security-model": (Fields.REQUIRED, Fields.Text()),
        "length-public-key": (Fields.REQUIRED, Fields.Integer()),
        "length-ciphertext": (Fields.REQUIRED, Fields.Integer()),
        "length-secret-key": (Fields.REQUIRED, Fields.Integer()),
        "length-shared-secret": (Fields.REQUIRED, Fields.Integer()),
        "length-keypair-seed": (Fields.OPTIONAL, Fields.Integer()),
        "length-encaps-seed": (Fields.OPTIONAL, Fields.Integer()),
        "nistkat-sha256": (Fields.OPTIONAL, Fields.Text()),
        "enable_by": (Fields.REQUIRED, Fields.Text()),
        # FIX: BIKE's implementations do not fit into the model of implementations
        #      so we cannot require default-implementation from parameter sets.
        #      Instead, we will need to check default-implementation in relational
        #      checks.
        "default-implementation": (Fields.OPTIONAL, Fields.Text()),
        "memopt-implementation": (Fields.OPTIONAL, Fields.Text()),
    }
)

SIG_PARAM_DATOR = Fields.Mapping(
    {
        "security-strength": (Fields.REQUIRED, Fields.Enumerated(SecStrength)),
        "security-model": (Fields.REQUIRED, Fields.Text()),
        "length-public-key": (Fields.REQUIRED, Fields.Integer()),
        "length-secret-key": (Fields.REQUIRED, Fields.Integer()),
        "length-signature": (Fields.REQUIRED, Fields.Integer()),
        "length-mu": (Fields.OPTIONAL, Fields.Integer()),
        "nistkat-sha256": (Fields.REQUIRED, Fields.Text()),
        "enable_by": (Fields.REQUIRED, Fields.Text()),
        "default-implementation": (Fields.REQUIRED, Fields.Text()),
        "memopt-implementation": (Fields.OPTIONAL, Fields.Text()),
    }
)

KEM_OR_SIG_PARAM_DATOR = Fields.Union({KEM_PARAM_DATOR, SIG_PARAM_DATOR})

UPSTREAMSRC_OR_COMMONSRC_DATOR = Fields.Union(
    {
        # Upstream sources
        Fields.Mapping(
            {
                "upstream": (Fields.REQUIRED, Fields.Text()),
                "base_dir": (Fields.OPTIONAL, Fields.Text()),
                "preserve_subdirs": (Fields.OPTIONAL, Fields.Boolean()),
                "files": (
                    Fields.REQUIRED,
                    Fields.Array(Fields.Text()),
                ),
            }
        ),
        # Family common sources
        Fields.Mapping(
            {
                "family_common": (Fields.REQUIRED, Fields.Text()),
            }
        ),
    }
)

COMMON_IMPL_MAPPING = {
    "version": (Fields.REQUIRED, Fields.Text()),
    "enable_by": (Fields.OPTIONAL, Fields.Text()),
    "subdirname": (Fields.OPTIONAL, Fields.Text()),
    "parameter": (Fields.REQUIRED, Fields.Text()),
    "uarch": (Fields.REQUIRED, Fields.Enumerated(UArch)),
    "common-crypto": (
        Fields.OPTIONAL,
        Fields.Mapping(
            {
                "AES": (Fields.OPTIONAL, Fields.Text()),
                "SHA3": (Fields.OPTIONAL, Fields.Text()),
            }
        ),
    ),
    "no-secret-dependent-branching-claimed": (Fields.REQUIRED, Fields.Boolean()),
    "no-secret-dependent-branching-checked-by-valgrind": (
        Fields.REQUIRED,
        Fields.Boolean(),
    ),
    "large-stack-usage": (Fields.REQUIRED, Fields.Boolean()),
    "os": (Fields.OPTIONAL, Fields.Array(Fields.Enumerated(SupportedOS))),
    "cpufeatures": (Fields.OPTIONAL, Fields.Array(Fields.Enumerated(CpuFeatures))),
    "sources": (Fields.REQUIRED, Fields.Array(UPSTREAMSRC_OR_COMMONSRC_DATOR)),
    "includes": (
        Fields.OPTIONAL,
        Fields.Array(
            Fields.Mapping(
                {
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeInterfaces)),
                    "dir": (Fields.REQUIRED, Fields.Text()),
                }
            )
        ),
    ),
    "old_gas_syntax_if_darwin": (Fields.OPTIONAL, Fields.Boolean()),
    "compile_opts": (
        Fields.OPTIONAL,
        Fields.Array(
            Fields.Mapping(
                {
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeInterfaces)),
                    "opts": (Fields.REQUIRED, Fields.Text()),
                }
            )
        ),
    ),
    "link_libs": (Fields.OPTIONAL, Fields.Array(Fields.Text(), allowempty=False)),
    "set_properties": (
        Fields.OPTIONAL,
        Fields.Array(
            Fields.Mapping(
                {
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeInterfaces)),
                    "name": (Fields.REQUIRED, Fields.Text()),
                    "value": (Fields.REQUIRED, Fields.Text()),
                }
            )
        ),
    ),
}

KEM_API_MAPPING = {
    "signature_keypair": (Fields.REQUIRED, Fields.Text()),
    "signature_enc": (Fields.REQUIRED, Fields.Text()),
    "signature_dec": (Fields.REQUIRED, Fields.Text()),
    "signature_keypair_derand": (Fields.OPTIONAL, Fields.Text()),
    "signature_enc_derand": (Fields.OPTIONAL, Fields.Text()),
}

SIG_API_MAPPING = {
    "signature_keypair": (Fields.REQUIRED, Fields.Text()),
    "signature_signature": (Fields.REQUIRED, Fields.Text()),
    "signature_verify": (Fields.REQUIRED, Fields.Text()),
    "signature_signature_extmu": (Fields.OPTIONAL, Fields.Text()),
    "signature_verify_extmu": (Fields.OPTIONAL, Fields.Text()),
}

KEM_IMPL_DATOR = Fields.Mapping(COMMON_IMPL_MAPPING | KEM_API_MAPPING)

SIG_IMPL_DATOR = Fields.Mapping(COMMON_IMPL_MAPPING | SIG_API_MAPPING)

KEM_OR_SIG_IMPL_DATOR = Fields.Union({KEM_IMPL_DATOR, SIG_IMPL_DATOR})

COMMON_SRC_DATOR = Fields.Mapping(
    {
        "upstream": (Fields.REQUIRED, Fields.Text()),
        "destdir": (Fields.OPTIONAL, Fields.Text()),
        "upstream_base_dir": (Fields.OPTIONAL, Fields.Text()),
        "files": (Fields.REQUIRED, Fields.Array(Fields.Text())),
    }
)

ALGFAMILY_DATOR = Fields.Mapping(
    {
        "displayname": (Fields.REQUIRED, Fields.Text()),
        "algtype": (Fields.REQUIRED, Fields.Enumerated(AlgTypes)),
        "principal-submitters": (Fields.REQUIRED, Fields.Array(Fields.Text(), False)),
        "auxiliary-submitters": (Fields.OPTIONAL, Fields.Array(Fields.Text())),
        "crypto-assumption": (Fields.REQUIRED, Fields.Text()),
        "website": (Fields.REQUIRED, Fields.Text()),
        "nist-round": (Fields.REQUIRED, Fields.Text()),
        "spec-version": (Fields.REQUIRED, Fields.Text()),
        "standardization-status": (Fields.REQUIRED, Fields.Text()),
        "upstream-maintenance": (Fields.REQUIRED, Fields.Text()),
        "upstream-statement-url": (Fields.OPTIONAL, Fields.Text()),
        # TODO: relational checks are too complex to implement in Fields; instead
        #       they will be checked at parsing
        "primary-upstream": (Fields.REQUIRED, Fields.Text()),
        "oqs-support-tier": (Fields.REQUIRED, Fields.Enumerated(OQSSupportTiers)),
        "upstream-ancestors": (Fields.OPTIONAL, Fields.Array(Fields.Text())),
        "advisories": (Fields.OPTIONAL, Fields.Array(Fields.Text())),
        # TODO: another relational check: if algtype is kem, then parameters must
        #       be keyed array of kem parameters.
        "parameters": (Fields.REQUIRED, Fields.KeyedArray(KEM_OR_SIG_PARAM_DATOR)),
        "common_src": (Fields.OPTIONAL, Fields.KeyedArray(COMMON_SRC_DATOR)),
        "implementations-switch-on-runtime-cpu-features": (
            Fields.OPTIONAL,
            Fields.Boolean(),
        ),
        "implementations": (Fields.OPTIONAL, Fields.KeyedArray(KEM_OR_SIG_IMPL_DATOR)),
    }
)


OQS_META_DATOR = Fields.Mapping(
    {
        "schema_version": (Fields.REQUIRED, Fields.Integer()),
        "upstreams": (Fields.REQUIRED, Fields.KeyedArray(UPSTREAM_DATOR)),
        "algfamilies": (Fields.REQUIRED, Fields.KeyedArray(ALGFAMILY_DATOR)),
    }
)


class RelationalChecks:
    @staticmethod
    def impl_key_exists(family_meta: dict) -> bool:
        impl_keys = family_meta.get("implementations", {}).keys()
        for param_key, param_meta in family_meta.get("parameters", {}):
            default_impl_key = param_meta.get("default-implementation", None)
            if default_impl_key and (default_impl_key not in impl_keys):
                logger.error(f"{param_key} default impl {default_impl_key} invalid")
                return False
        return True

    @staticmethod
    def param_key_exists(family_meta: dict) -> bool:
        """The "param" field in an implementation must match a param key in the same
        family
        """
        raise NotImplementedError()

    @staticmethod
    def common_src_key_exists(family_meta: dict) -> bool:
        """If an implementation uses a set of common source files, the reference
        must match a common_src_key in the same family
        """
        raise NotImplementedError()

    @staticmethod
    def run_all() -> bool:
        raise NotImplementedError()


if __name__ == "__main__":
    builderconfig = OQSBuilderConfig()

    with open(builderconfig.oqs_meta_path) as f:
        oqs_meta = yaml.safe_load(f)
    for demo_algfamily in builderconfig.demo_algfamilies:
        demo_algfamily_meta = oqs_meta["algfamilies"].pop(demo_algfamily)
        if demo_algfamily_meta:
            logger.info(f"Removed demo alg {demo_algfamily}")

    assert OQS_META_DATOR.validate(oqs_meta), "Error"
