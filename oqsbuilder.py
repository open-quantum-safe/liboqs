#!/usr/bin/env python3

import abc
import argparse
from enum import Enum, EnumType
import logging
import os
import pathlib
import shutil
import subprocess
import sys
from typing import Any, Mapping
from tempfile import TemporaryDirectory
import warnings

import yaml
import jinja2

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
LIBOQS_SRC_DIR = os.path.join(LIBOQS_DIR, "src")
DEFAULT_OQS_META_PATH = os.path.join(LIBOQS_DIR, "OQS_META.yml")
DEFAULT_UPSTREAMS_DIR = os.path.join(LIBOQS_DIR)
DEFAULT_KEM_SRC_TEMPLATE_FILENAME = "kem.c.jinja"
DEFAULT_SIG_SRC_TEMPLATE_FILENAME = "sig.c.jinja"
DEFAULT_KEM_HEADER_TEMPLATE_FILENAME = "kem.h.jinja"
DEFAULT_SIG_HEADER_TEMPLATE_FILENAME = "sig.h.jinja"
DEFAULT_LISTFILE_TEMPLATE_FILENAME = "CMakeLists.txt.jinja"
DEFAULT_PATCH_DIR = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
DEFAULT_OQS_TEMPLATES_DIR = os.path.join(LIBOQS_DIR, "templates")
DEFAULT_NEVER_COPY = [
    "Makefile.Microsoft_nmake",
    "Makefile",
]
SOURCE_FILE_EXTENSIONS = [
    ".c",
    ".s",
    ".S",
    ".cu",
    ".cpp",
]
CLI_ARGS_LOGLEVEL_COICES = ["debug", "info", "warning", "error"]

def cli_loglevel_choices_to_loglevel(choice: str):
    assert choice in CLI_ARGS_LOGLEVEL_COICES, f"Invalid log level {choice}"
    match choice:
        case "debug":
            return logging.DEBUG
        case "info":
            return logging.INFO
        case "warning":
            return logging.WARNING
        case "error":
            return logging.ERROR
    raise KeyError(f"Invalid log level {choice}")


class OQSBuilderConfig:
    def __init__(
        self,
        oqs_meta_path: str = DEFAULT_OQS_META_PATH,
        upstreams_dir: str = DEFAULT_UPSTREAMS_DIR,
        patch_dir: str = DEFAULT_PATCH_DIR,
        templates_dir: str = DEFAULT_OQS_TEMPLATES_DIR,
        kem_src_template_filename: str = DEFAULT_KEM_SRC_TEMPLATE_FILENAME,
        sig_src_template_filename: str = DEFAULT_SIG_SRC_TEMPLATE_FILENAME,
        kem_header_template_filename: str = DEFAULT_KEM_HEADER_TEMPLATE_FILENAME,
        sig_header_template_filename: str = DEFAULT_SIG_HEADER_TEMPLATE_FILENAME,
        listfile_template_filename: str = DEFAULT_LISTFILE_TEMPLATE_FILENAME,
        fail_on_jasminc: bool = False,
        never_copy: list[str] = DEFAULT_NEVER_COPY,
    ):
        """
        :param upstreams_dir: Specify a directory to clone upstream repositories
            into. Defaults to $LIBOQS_DIR
        :param patch_dir: Specify the directory that hosts all the patch files.
            Defaults to LIBOQS_DIR/scripts/copy_from_upstream/patches
        :param fail_on_jasminc: If True, do a hard fail for when jasminc is
            not present or the version is incorrect. If False, do not clone
            libjade, do not refresh libjade implementations
        :param never_copy: a list of glob patterns to exclude upstream files
            from being copied
        """
        self.oqs_meta_path = oqs_meta_path
        self.upstreams_dir = upstreams_dir
        self.upstreams_cached = False
        self.patch_dir = patch_dir
        self.templates_dir = templates_dir
        self.kem_src_template_filename = kem_src_template_filename
        self.sig_src_template_filename = sig_src_template_filename
        self.kem_header_template_filename = kem_header_template_filename
        self.sig_header_template_filename = sig_header_template_filename
        self.listfile_template_filename = listfile_template_filename
        self.delete_upstreams = True
        self.demo_algfamilies = ["demo_alg"]
        self.fail_on_jasminc = fail_on_jasminc
        self.never_copy = never_copy

    def overwrite_with_cli_args(self, args: argparse.Namespace):
        if args.upstreams_dir:
            self.upstreams_dir = args.upstreams_dir
            self.upstreams_cached = True
        if args.oqs_meta:
            if not os.path.isfile(args.oqs_meta):
                raise FileNotFoundError(f"{args.oqs_meta} not found")
            self.oqs_meta_path = args.oqs_meta
        self.delete_upstreams = not args.keep_upstreams


class FieldValidator(abc.ABC):
    # TODO: this method should return (ok, errmsg_or_none)
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
                    logger.error("failed to validate %s", key)
                    return False
                if (subtarget is None) and required:
                    logger.error("missing key %s", key)
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

    @property
    def dir(self):
        """Return <liboqs_dir>/src/<kem|sig|stfl_sig> according to variant"""
        path = os.path.join(LIBOQS_SRC_DIR, self.value)
        if not os.path.isdir(path):
            raise FileNotFoundError(f"{path} not found")
        return path


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

    def compile_time_guard(self) -> str | None:
        match self:
            case UArch.Cuda:
                return "OQS_USE_CUPQC"
            case UArch.Icicle:
                return "OQS_USE_ICICLE"
        return None


class SupportedOS(Enum):
    Linux = "Linux"
    Darwin = "Darwin"


class CpuFeatures(Enum):
    AVX2 = "avx2"
    BMI2 = "bmi2"
    POPCNT = "popcnt"
    NEON = "asimd"
    AES = "aes"

    def to_oqs_macro(self) -> str:
        match self:
            case CpuFeatures.AVX2:
                return "OQS_CPU_EXT_AVX2"
            case CpuFeatures.BMI2:
                return "OQS_CPU_EXT_BMI2"
            case CpuFeatures.POPCNT:
                return "OQS_CPU_EXT_POPCNT"
            case CpuFeatures.NEON:
                return "OQS_CPU_EXT_ARM_NEON"
            case CpuFeatures.AES:
                return "OQS_CPU_EXT_AES"


class CmakeScopes(Enum):
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
        # TODO: BIKE's implementations do not fit into the model of implementations
        #   so we cannot require default-implementation from parameter sets.
        #   Instead, we will need to check default-implementation in relational
        #   checks.
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
        "sign-with-ctx": (Fields.REQUIRED, Fields.Boolean()),
        "default-implementation": (Fields.OPTIONAL, Fields.Text()),
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
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeScopes)),
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
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeScopes)),
                    "opts": (Fields.REQUIRED, Fields.Text()),
                }
            )
        ),
    ),
    "link_libs": (
        Fields.OPTIONAL,
        Fields.Array(
            Fields.Mapping(
                {
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeScopes)),
                    "libs": (Fields.REQUIRED, Fields.Text()),
                }
            )
        ),
    ),
    "set_properties": (
        Fields.OPTIONAL,
        Fields.Array(
            Fields.Mapping(
                {
                    "scope": (Fields.REQUIRED, Fields.Enumerated(CmakeScopes)),
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
        "jasminc_version": (Fields.REQUIRED, Fields.Text()),
        "upstreams": (Fields.REQUIRED, Fields.KeyedArray(UPSTREAM_DATOR)),
        "algfamilies": (Fields.REQUIRED, Fields.KeyedArray(ALGFAMILY_DATOR)),
    }
)


def run_subprocess(
    cmd: list[str], cwd: str | None = None, expected_ret=0, dryrun=False
):
    """Run a command in a sub-process, pipe stdout and stderr to logger."""
    if dryrun:
        logger.info("Subprocess call: `%s`", " ".join(cmd))
        return
    else:
        logger.debug("Subprocess call: `%s`", " ".join(cmd))

    proc = subprocess.Popen(
        cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )
    assert proc.stdout is not None

    for line in proc.stdout:
        logger.debug("%s", line.rstrip())

    ret = proc.wait()
    if ret != expected_ret:
        raise subprocess.CalledProcessError(ret, cmd)


class UpstreamMeta:
    def __init__(
        self,
        git_url: str,
        git_commit: str,
        patch_full_paths: list[str],
        post_patches: str | None,
    ):
        self.git_url = git_url
        self.git_commit = git_commit
        self.patch_full_paths = patch_full_paths
        self.post_patches = post_patches

        self._dir: str | None = None
        self._patched = False
        self._posted = False

    def dir(self, upstream_key: str) -> str:
        if not self._dir:
            raise ValueError(f"Upstream {upstream_key} was not cloned")
        return self._dir

    def match_path_patterns(
        self,
        upstream_key: str,
        patterns: list[str],
        base_dir: str | None,
        excludes: list[str],
    ) -> list[str]:
        """Return a list of paths relative to <self._dir>/<base_dir> that match
        any of the input patterns
        """
        full_base_dir = (
            pathlib.Path(self.dir(upstream_key)) / base_dir
            if base_dir
            else pathlib.Path(self.dir(upstream_key))
        )
        matches = []
        for pattern in patterns:
            pattern_matches = [
                path
                for path in list(full_base_dir.glob(pattern))
                if not any([path.match(exclude) for exclude in excludes])
            ]
            logger.debug(
                "%d matches %s in %s",
                len(pattern_matches),
                pattern,
                str(full_base_dir),
            )
            for match in list(pattern_matches):
                matches.append(str(match.relative_to(full_base_dir)))

        return matches

    @staticmethod
    def from_dict(patch_dir: str, meta: dict):
        patch_full_paths = []
        for patch_name in meta.get("patches", []):
            patch_path = os.path.join(patch_dir, patch_name)
            if not os.path.isfile(patch_path):
                raise FileNotFoundError(f"Cannot find patch {patch_path}")
            patch_full_paths.append(patch_path)

        return UpstreamMeta(
            meta["git_url"],
            meta["git_commit"],
            patch_full_paths,
            meta.get("post_patches", None),
        )

    def clone_and_patch(self, upstream_key: str, upstreams_dir: str):
        """Clone the specified commit into <parent_dir>/<upstream_key>,
        then set self._dir to this path, indicating successful cloning
        """
        upstream_dir = os.path.join(upstreams_dir, upstream_key)
        self._dir = upstream_dir
        if os.path.isdir(upstream_dir):
            logger.info("Using cached upstream %s at %s", upstream_key, upstream_dir)
            return
        os.mkdir(upstream_dir)

        run_subprocess(["git", "init"], upstream_dir)
        run_subprocess(["git", "remote", "add", "origin", self.git_url], upstream_dir)
        run_subprocess(
            ["git", "fetch", "--depth=1", "origin", self.git_commit], upstream_dir
        )
        run_subprocess(["git", "checkout", "--detach", "FETCH_HEAD"], upstream_dir)

        if self.patch_full_paths:
            run_subprocess(
                ["git", "apply", "--whitespace=fix"] + self.patch_full_paths, self._dir
            )

        if self.post_patches:
            cmd_lines = self.post_patches.strip().splitlines()
            for cmd_line in cmd_lines:
                run_subprocess(cmd_line.split(), self._dir)


class KemParameterMeta:
    def __init__(
        self,
        strength: SecStrength,
        model: str,
        length_publickey: int,
        length_ciphertext: int,
        length_secretkey: int,
        length_sharedsecret: int,
        length_keypair_seed: int | None,
        length_encaps_seed: int | None,
        nistkat_sha256: str | None,
        enable_by: str,
        default_impl: str | None,
        memopt_impl: str | None,
    ):
        self.strength = strength
        self.model = model
        self.length_publickey = length_publickey
        self.length_ciphertext = length_ciphertext
        self.length_secretkey = length_secretkey
        self.length_sharedsecret = length_sharedsecret
        self.length_keypair_seed = length_keypair_seed
        self.length_encaps_seed = length_encaps_seed
        self.nistkat_sha256 = nistkat_sha256
        self.enable_by = enable_by
        self.default_impl = default_impl
        self.memopt_impl = memopt_impl

    @staticmethod
    def from_dict(meta: dict):
        strength = SecStrength(meta["security-strength"])
        model = meta["security-model"]
        pubkeylen = meta["length-public-key"]
        ciphertextlen = meta["length-ciphertext"]
        privkeylen = meta["length-secret-key"]
        secretlen = meta["length-shared-secret"]
        keypair_seed = meta.get("length-keypair-seed", None)
        encaps_seed = meta.get("length-encaps-seed", None)
        nistkat = meta.get("nistkat-sha256", None)
        enable_by = meta["enable_by"]
        default_impl = meta.get("default-implementation", None)
        memopt_impl = meta.get("memopt-implementation", None)
        return KemParameterMeta(
            strength,
            model,
            pubkeylen,
            ciphertextlen,
            privkeylen,
            secretlen,
            keypair_seed,
            encaps_seed,
            nistkat,
            enable_by,
            default_impl,
            memopt_impl,
        )


class SigParameterMeta:
    def __init__(
        self,
        strength: SecStrength,
        model: str,
        length_publickey: int,
        length_secretkey: int,
        length_signature: int,
        length_mu: int | None,
        nistkat_sha256: str | None,
        enable_by: str,
        sign_with_ctx: bool,
        default_impl: str | None,
        memopt_impl: str | None,
    ):
        self.strength = strength
        self.model = model
        self.length_publickey = length_publickey
        self.length_secretkey = length_secretkey
        self.length_signature = length_signature
        self.length_mu = length_mu
        self.nistkat_sha256 = nistkat_sha256
        self.enable_by = enable_by
        self.sign_with_ctx = sign_with_ctx
        self.default_impl = default_impl
        self.memopt_impl = memopt_impl

    def is_euf_cma(self) -> bool:
        # We will probably never get a scheme that is not even EUF-CMA
        return True

    def is_suf_cma(self) -> bool:
        """SUF-CMA is strictly stronger than EUF-CMA"""
        return self.model == "SUF-CMA"

    def supports_ctx(self):
        return self.sign_with_ctx

    @staticmethod
    def from_dict(meta: dict):
        strength = SecStrength(meta["security-strength"])
        model = meta["security-model"]
        pubkeylen = meta["length-public-key"]
        privkeylen = meta["length-secret-key"]
        siglen = meta["length-signature"]
        mulen = meta.get("length-mu", None)
        nistkat = meta.get("nistkat-sha256", None)
        enable_by = meta["enable_by"]
        sign_with_ctx = meta["sign-with-ctx"]
        default_impl = meta.get("default-implementation", None)
        memopt_impl = meta.get("memopt-implementation", None)
        return SigParameterMeta(
            strength,
            model,
            pubkeylen,
            privkeylen,
            siglen,
            mulen,
            nistkat,
            enable_by,
            sign_with_ctx,
            default_impl,
            memopt_impl,
        )


class CommonSrcMeta:
    # TODO: common source files current do not support preserving subdirectories
    #       in the same way implementation source files can. I don't plan to
    #       support it unless there is a strong case
    def __init__(
        self,
        upstream_key: str,
        destdir: str | None,
        upstream_base_dir: str | None,
        patterns: list[str],
    ):
        """
        :param patterns: a list of file patterns (support wildcards) to match
            against upstream repository
        """
        self.upstream_key = upstream_key
        self.destdir = destdir
        self.upstream_base_dir = upstream_base_dir
        self.patterns = patterns

        self._paths: list[str] | None = None

    def set_paths(
        self,
        algtype: AlgTypes,
        algfamily_key: str,
        common_src_dirname: str,
        paths: list[str],
    ):
        """Set self._files to the precise list of files in this set of
        family-level common source files. The paths are relative to
        $LIBOQS_DIR/src/<algtype>/<algfamily>/<common_key>
        """
        base_dir = os.path.join(
            LIBOQS_DIR, "src", algtype.value, algfamily_key, common_src_dirname
        )
        for path in paths:
            fullpath = os.path.join(base_dir, path)
            if not os.path.isfile(fullpath):
                raise FileNotFoundError(f"{fullpath} not found")
        self._paths = paths

    def get_rel_paths(self) -> list[str]:
        """Return the list of paths to source files under this set of family-level
        common source files. Each path is relative to
        <liboqs_dir>/src/<algtype>/<family>/<common_src_dirname>
        """
        if not self._paths:
            raise ValueError("Common source files not copied")
        return self._paths

    def get_full_paths(
        self, algtype: AlgTypes, algfamily_key: str, common_src_key: str
    ) -> list[str]:
        """Return the list of paths to source files. Each path is absolute"""
        full_paths = []
        for rel_path in self.get_rel_paths():
            full_path = os.path.join(
                algtype.dir, algfamily_key, self.destdir or common_src_key, rel_path
            )
            if not os.path.isfile(full_path):
                raise FileNotFoundError(f"{full_path} not found")
            full_paths.append(full_path)
        return full_paths

    @staticmethod
    def from_dict(meta: dict):
        upstream_key = meta["upstream"]
        destdir = meta.get("destdir", None)
        upstream_base_dir = meta.get("upstream_base_dir", None)
        patterns = meta["files"]
        return CommonSrcMeta(upstream_key, destdir, upstream_base_dir, patterns)


class ImplSrcMeta:
    def __init__(
        self,
        upstream_key: str,
        base_dir: str | None,
        preserve_subdirs: bool,
        patterns: list[str],
    ):
        self.upstream_key = upstream_key
        self.base_dir = base_dir
        self.preserve_subdirs = preserve_subdirs
        self.patterns = patterns

        self._paths: list[str] | None = None

    def get_rel_paths(self) -> list[str]:
        if not self._paths:
            raise ValueError("Paths not set")
        return self._paths

    def get_full_paths(
        self, algtype: AlgTypes, algfamily_key: str, impl_src_dirname: str
    ) -> list[str]:
        """Return the list of paths to source files. Each path is absolute"""
        full_paths = []
        for rel_path in self.get_rel_paths():
            full_path = os.path.join(
                algtype.dir, algfamily_key, impl_src_dirname, rel_path
            )
            if not os.path.isfile(full_path):
                raise FileNotFoundError(f"{full_path} not found")
            full_paths.append(full_path)
        return full_paths

    def set_paths(
        self,
        algtype: AlgTypes,
        algfamily_key: str,
        impl_src_dirname: str,
        impl_src_rel_paths: list[str],
    ):
        """
        :param paths: a list of paths relative to
        $LIBOQS_DIR/src/<algtype>/<algfamily>/<impl_src_dirname>
        """
        base_dir = os.path.join(
            LIBOQS_DIR, "src", algtype.value, algfamily_key, impl_src_dirname
        )
        for path in impl_src_rel_paths:
            fullpath = os.path.join(base_dir, path)
            if not os.path.isfile(fullpath):
                raise FileNotFoundError(f"{fullpath} not found")
        self._paths = impl_src_rel_paths

    @staticmethod
    def from_dict(meta: dict):
        upstream = meta["upstream"]
        base_dir = meta.get("base_dir", None)
        preserve_subdirs = meta.get("preserve_subdirs", False)
        patterns = meta["files"]
        return ImplSrcMeta(upstream, base_dir, preserve_subdirs, patterns)


class KemApiMeta:
    def __init__(
        self,
        signature_keypair: str,
        signature_enc: str,
        signature_dec: str,
        signature_keypair_derand: str | None,
        signature_enc_derand: str | None,
    ):
        self.signature_keypair = signature_keypair
        self.signature_enc = signature_enc
        self.signature_dec = signature_dec
        self.signature_keypair_derand = signature_keypair_derand
        self.signature_enc_derand = signature_enc_derand


class SigApiMeta:
    def __init__(
        self,
        signature_keypair: str,
        signature_signature: str,
        signature_verify: str,
        signature_signature_extmu: str | None,
        signature_verify_extmu: str | None,
    ):
        self.signature_keypair = signature_keypair
        self.signature_signature = signature_signature
        self.signature_verify = signature_verify
        self.signature_signature_extmu = signature_signature_extmu
        self.signature_verify_extmu = signature_verify_extmu


class CMakeIncludeMeta:
    def __init__(
        self,
        scope: CmakeScopes,
        dir: str,
    ):
        self.scope = scope
        self.dir = dir

    @staticmethod
    def from_dict(meta: dict):
        scope = CmakeScopes(meta["scope"])
        dir = meta["dir"]
        return CMakeIncludeMeta(scope, dir)


class CMakeCompileOptMeta:
    def __init__(
        self,
        scope: CmakeScopes,
        opts: str,
    ):
        self.scope = scope
        self.opts = opts

    @staticmethod
    def from_dict(meta: dict):
        scope = CmakeScopes(meta["scope"])
        opts = meta["opts"]
        return CMakeCompileOptMeta(scope, opts)


class CMakeLinkLibsMeta:
    def __init__(
        self,
        scope: CmakeScopes,
        libs: str,
    ):
        self.scope = scope
        self.libs = libs

    @staticmethod
    def from_dict(meta: dict):
        scope = CmakeScopes(meta["scope"])
        libs = meta["libs"]
        return CMakeLinkLibsMeta(scope, libs)


class CMakeSetPropMeta:
    def __init__(
        self,
        scope: CmakeScopes,
        name: str,
        value: str,
    ):
        self.scope = scope
        self.name = name
        self.value = value

    @staticmethod
    def from_dict(meta: dict):
        scope = CmakeScopes(meta["scope"])
        name = meta["name"]
        value = meta["value"]
        return CMakeSetPropMeta(scope, name, value)


class CommonSrcRef:
    def __init__(self, common_src_key: str):
        self.common_src_key = common_src_key


class ImplementationMeta:
    def __init__(
        self,
        version: str,
        enable_by: str | None,
        subdirname: str | None,
        parameter: str,
        uarch: UArch,
        common_crypto: dict[str, str],
        no_secret_dep_branching_claimed: bool,
        no_secret_dep_branching_valgrind: bool,
        large_stack: bool,
        supported_os: list[SupportedOS],
        cpufeatures: list[CpuFeatures],
        sources: list[ImplSrcMeta | CommonSrcRef],
        includes: list[CMakeIncludeMeta],
        old_gas_if_darwin: bool,
        compile_opts: list[CMakeCompileOptMeta],
        link_libs: list[CMakeLinkLibsMeta],
        set_properties: list[CMakeSetPropMeta],
        api: KemApiMeta | SigApiMeta,
    ):
        self.version = version
        self.enable_by = enable_by
        self.subdirname = subdirname
        self.parameter = parameter
        self.uarch = uarch
        self.common_crypto = common_crypto
        self.no_secret_dep_branching_claimed = no_secret_dep_branching_claimed
        self.no_secret_dep_branching_valgrind = no_secret_dep_branching_valgrind
        self.large_stack = large_stack
        self.supported_os = supported_os
        self.cpufeatures = cpufeatures
        self.sources = sources
        self.includes = includes
        self.old_gas_if_darwin = old_gas_if_darwin
        self.compile_opts = compile_opts
        self.link_libs = link_libs
        self.set_properties = set_properties
        self.api = api

    def render_runtime_cpu_feature_gate(self) -> str | None:
        if not self.cpufeatures:
            return None
        return " && ".join(
            [
                f"OQS_CPU_has_extension({feat.to_oqs_macro()})"
                for feat in self.cpufeatures
            ]
        )

    def is_libjade_impl(self) -> bool:
        """Return True iff any source comes from libjade"""
        return any(
            [
                isinstance(src, ImplSrcMeta) and src.upstream_key == "libjade"
                for src in self.sources
            ]
        )

    def copy_sources_sets(
        self,
        algtype: AlgTypes,
        algfamily_key: str,
        impl_key: str,
        upstreams: dict[str, UpstreamMeta],
        excludes: list[str],
        has_jasmin: bool,
    ):
        """Copy this implementation's source files"""
        if self.is_libjade_impl() and (not has_jasmin):
            logger.warning("Impl %s is skipped due to missing jasminc", impl_key)
            return
        for impl_src in self.sources:
            if isinstance(impl_src, CommonSrcRef):
                continue
            upstream_meta = upstreams[impl_src.upstream_key]
            impl_src_dirname = self.subdirname or impl_key
            impl_src_dirpath = os.path.join(
                algtype.dir, algfamily_key, impl_src_dirname
            )
            if os.path.isdir(impl_src_dirpath):
                logger.warning("Destructively refreshing %s", impl_src_dirpath)
                shutil.rmtree(impl_src_dirpath)
            os.makedirs(impl_src_dirpath)
            upstream_rel_paths = upstream_meta.match_path_patterns(
                impl_src.upstream_key,
                impl_src.patterns,
                impl_src.base_dir,
                excludes,
            )
            impl_src_rel_paths = []
            for upstream_rel_path in upstream_rel_paths:
                upstream_full_path = os.path.join(
                    upstream_meta.dir(impl_src.upstream_key),
                    impl_src.base_dir or "",
                    upstream_rel_path,
                )
                if os.path.isdir(upstream_full_path):
                    raise NotImplementedError(
                        f"{upstream_full_path} is a directory. OQSBuilder "
                        "currently does not support recursively copying "
                        "subdirectories from upstream. Use explicit set of "
                        f"files or glob pattern {upstream_full_path}/* instead."
                    )
                if not os.path.isfile(upstream_full_path):
                    raise ValueError(f"{upstream_full_path} is invalid path")
                _, filename = os.path.split(upstream_full_path)
                impl_src_rel_path = (
                    upstream_rel_path if impl_src.preserve_subdirs else filename
                )
                impl_src_full_path = os.path.join(impl_src_dirpath, impl_src_rel_path)
                # Because of preserve_subdirs, impl_src_full_path may contain
                # intermediate subdirectories that don't exist yet
                impl_src_inter_dir, _ = os.path.split(impl_src_full_path)
                if not os.path.isdir(impl_src_inter_dir):
                    logger.warning(
                        "Creating intermediate directories %s", impl_src_inter_dir
                    )
                    os.makedirs(impl_src_inter_dir)
                shutil.copy2(upstream_full_path, impl_src_full_path)
                logger.debug(
                    "Copied from %s to %s", upstream_full_path, impl_src_full_path
                )
                impl_src_rel_paths.append(impl_src_rel_path)
            impl_src.set_paths(
                algtype, algfamily_key, impl_src_dirname, impl_src_rel_paths
            )
            logger.debug(
                "Copied %d files into %s",
                len(impl_src.get_rel_paths()),
                impl_src_dirname,
            )

    @staticmethod
    def parse_src_meta(src_meta: dict):
        if "upstream" in src_meta:
            return ImplSrcMeta.from_dict(src_meta)
        if "family_common" in src_meta:
            return CommonSrcRef(src_meta["family_common"])
        raise KeyError("Invalid entry in implementation source list")

    @staticmethod
    def parse_api(algtype: AlgTypes, impl_meta: dict):
        if algtype == AlgTypes.Kem:
            return KemApiMeta(
                impl_meta["signature_keypair"],
                impl_meta["signature_enc"],
                impl_meta["signature_dec"],
                impl_meta.get("signature_keypair_derand", None),
                impl_meta.get("signature_enc_derand", None),
            )
        if algtype == AlgTypes.Sig:
            return SigApiMeta(
                impl_meta["signature_keypair"],
                impl_meta["signature_signature"],
                impl_meta["signature_verify"],
                impl_meta.get("signature_signature_extmu", None),
                impl_meta.get("signature_verify_extmu", None),
            )
        raise ValueError(f"Invalid algtype {algtype}")

    @staticmethod
    def from_dict(algtype: AlgTypes, meta: dict):
        version = meta["version"]
        enable_by = meta.get("enable_by", None)
        subdirname = meta.get("subdirname", None)
        param_key = meta["parameter"]
        uarch = UArch(meta["uarch"])
        common_crypto = meta.get("common-crypto", {})
        no_secret_dep_branching_claimed = meta["no-secret-dependent-branching-claimed"]
        no_secret_dep_branching_valgrind = meta[
            "no-secret-dependent-branching-checked-by-valgrind"
        ]
        large_stack = meta["large-stack-usage"]
        supported_os = [SupportedOS(os) for os in meta.get("os", [])]
        cpufeatures = [CpuFeatures(feat) for feat in meta.get("cpufeatures", [])]
        sources = [
            ImplementationMeta.parse_src_meta(src_meta)
            for src_meta in meta.get("sources", [])
        ]
        includes = [
            CMakeIncludeMeta.from_dict(include) for include in meta.get("includes", [])
        ]
        old_gas = meta.get("old_gas_syntax_if_darwin", False)
        compile_opts = [
            CMakeCompileOptMeta.from_dict(compile_opt)
            for compile_opt in meta.get("compile_opts", [])
        ]
        link_libs = [
            CMakeLinkLibsMeta.from_dict(link_libs_meta)
            for link_libs_meta in meta.get("link_libs", [])
        ]
        props = [
            CMakeSetPropMeta.from_dict(propmeta)
            for propmeta in meta.get("set_properties", [])
        ]
        api = ImplementationMeta.parse_api(algtype, meta)
        return ImplementationMeta(
            version,
            enable_by,
            subdirname,
            param_key,
            uarch,
            common_crypto,
            no_secret_dep_branching_claimed,
            no_secret_dep_branching_valgrind,
            large_stack,
            supported_os,
            cpufeatures,
            sources,
            includes,
            old_gas,
            compile_opts,
            link_libs,
            props,
            api,
        )


class AlgFamilyMeta:
    """A direct translation of algorithm family metadata"""

    def __init__(
        self,
        displayname: str,
        algtype: AlgTypes,
        principal_submitters: list[str],
        auxiliary_submitters: list[str],
        crypto_assumptions: str,
        website: str,
        nist_round: str,
        spec_version: str,
        standardization_status: str,
        upstream_maintenance: str,
        upstream_statement_url: str | None,
        primary_upstream: str,
        oqs_support_tier: OQSSupportTiers,
        upstream_ancestors: list[str],
        advisories: list[str],
        parameters: dict[str, KemParameterMeta | SigParameterMeta],
        switch_on_runtime_cpu_features: bool,
        common_src: dict[str, CommonSrcMeta],
        implementations: dict[str, ImplementationMeta],
    ):
        self.displayname = displayname
        self.algtype = algtype
        self.principal_submitters = principal_submitters
        self.auxiliary_submitters = auxiliary_submitters
        self.crypto_assumptions = crypto_assumptions
        self.website = website
        self.nist_round = nist_round
        self.spec_version = spec_version
        self.standardization_status = standardization_status
        self.upstream_maintenance = upstream_maintenance
        self.upstream_statement_url = upstream_statement_url
        self.primary_upstream = primary_upstream
        self.oqs_support_tier = oqs_support_tier
        self.upstream_ancestors = upstream_ancestors
        self.advisories = advisories
        self.parameters = parameters
        self.switch_on_runtime_cpu_features = switch_on_runtime_cpu_features
        self.common_src = common_src
        self.implementations = implementations

    @staticmethod
    def from_dict(family_meta: dict):
        displayname = family_meta["displayname"]
        algtype = AlgTypes(family_meta["algtype"])
        principal = family_meta["principal-submitters"]
        aux = family_meta.get("auxiliary-submitters", [])
        assume = family_meta["crypto-assumption"]
        website = family_meta["website"]
        nist_round = family_meta["nist-round"]
        spec_version = family_meta["spec-version"]
        standardization_status = family_meta["standardization-status"]
        upstream_maintenance = family_meta["upstream-maintenance"]
        upstream_statement_url = family_meta.get("upstream-statement-url", None)
        primary_upstream = family_meta["primary-upstream"]
        oqs_support_tier = OQSSupportTiers(family_meta["oqs-support-tier"])
        upstream_ancestors = family_meta.get("upstream-ancestors", [])
        advisories = family_meta.get("advisories", [])
        parameters = {
            key: (
                KemParameterMeta.from_dict(param_meta)
                if algtype == AlgTypes.Kem
                else SigParameterMeta.from_dict(param_meta)
            )
            for key, param_meta in family_meta["parameters"].items()
        }
        switch_on_runtime_cpu_features = family_meta.get(
            "implementations-switch-on-runtime-cpu-features", True
        )
        common_src = {
            key: CommonSrcMeta.from_dict(common_src_meta)
            for key, common_src_meta in family_meta.get("common_src", {}).items()
        }
        implementations = {
            key: ImplementationMeta.from_dict(algtype, impl_meta)
            for key, impl_meta in family_meta.get("implementations", {}).items()
        }

        return AlgFamilyMeta(
            displayname,
            algtype,
            principal,
            aux,
            assume,
            website,
            nist_round,
            spec_version,
            standardization_status,
            upstream_maintenance,
            upstream_statement_url,
            primary_upstream,
            oqs_support_tier,
            upstream_ancestors,
            advisories,
            parameters,
            switch_on_runtime_cpu_features,
            common_src,
            implementations,
        )

    def list_source_files(self, impl_key: str) -> list[str]:
        """Return the paths of source files to be compiled into the specified
        implementation
        """
        algfamily_rel_paths = []
        impl_meta = self.implementations[impl_key]
        for src_meta in impl_meta.sources:
            if isinstance(src_meta, CommonSrcRef):
                common_src_meta = self.common_src[src_meta.common_src_key]
                common_src_dirname = common_src_meta.destdir or src_meta.common_src_key
                for common_src_rel_path in common_src_meta.get_rel_paths():
                    algfamily_rel_paths.append(
                        os.path.join(common_src_dirname, common_src_rel_path)
                    )
            elif isinstance(src_meta, ImplSrcMeta):
                impl_dirname = impl_meta.subdirname or impl_key
                for impl_rel_path in src_meta.get_rel_paths():
                    algfamily_rel_paths.append(
                        os.path.join(impl_dirname, impl_rel_path)
                    )
            else:
                raise ValueError(f"Invalid source meta under {impl_key}")

        algfamily_rel_paths = [
            f
            for f in algfamily_rel_paths
            if any([f.endswith(ext) for ext in SOURCE_FILE_EXTENSIONS])
        ]
        return algfamily_rel_paths

    def list_params_impls(
        self, param_key: str, exclude_default: bool
    ) -> dict[str, ImplementationMeta]:
        """Return the subset of the implementations matching the input param_key"""
        return {
            k: m
            for k, m in self.implementations.items()
            if m.parameter == param_key
            and ((not exclude_default) or k != self.parameters[param_key].default_impl)
        }

    def copy_common_src(
        self,
        algfamily_key: str,
        upstreams: dict[str, UpstreamMeta],
        excludes: list[str],
    ):
        """Iterate through the set of family-level common sources. For each set
        re-create the subdirectory, then copy the files. Modify self.common_src
        to take precise inventory of the files copied.
        """
        if not self.common_src:
            return
        for common_src_key, common_src_meta in self.common_src.items():
            upstream_meta = upstreams[common_src_meta.upstream_key]
            common_src_dirname = common_src_meta.destdir or common_src_key
            common_src_dirpath = os.path.join(
                self.algtype.dir, algfamily_key, common_src_dirname
            )
            if os.path.isdir(common_src_dirpath):
                logger.warning("Destructively refreshing %s", common_src_dirpath)
                shutil.rmtree(common_src_dirpath)
            os.makedirs(common_src_dirpath)
            # upstream_rel_paths are relative to <upstream>/<base_dir>
            upstream_rel_paths = upstream_meta.match_path_patterns(
                common_src_meta.upstream_key,
                common_src_meta.patterns,
                common_src_meta.upstream_base_dir,
                excludes,
            )
            common_src_rel_paths = []
            for upstream_rel_path in upstream_rel_paths:
                _, filename = os.path.split(upstream_rel_path)
                upstream_full_path = os.path.join(
                    upstream_meta.dir(common_src_meta.upstream_key),
                    common_src_meta.upstream_base_dir or "",
                    upstream_rel_path,
                )
                # I did this roundabout assignment in case we need to implement
                # preserve_subdirectory for common_src, in which case
                # common_src_rel_path is more than just filename
                common_src_rel_path = filename
                common_src_full_path = os.path.join(
                    common_src_dirpath, common_src_rel_path
                )
                shutil.copy2(upstream_full_path, common_src_full_path)
                logger.debug(
                    "Copied from %s to %s", upstream_full_path, common_src_full_path
                )
                common_src_rel_paths.append(common_src_rel_path)
            common_src_meta.set_paths(
                self.algtype, algfamily_key, common_src_dirname, common_src_rel_paths
            )
            logger.debug(
                "Copied %d files into %s",
                len(common_src_meta.get_rel_paths()),
                common_src_dirpath,
            )

    def copy_implementations_src(
        self,
        algfamily_key: str,
        upstreams: dict[str, UpstreamMeta],
        excludes: list[str],
        has_jasmin: bool,
    ):
        """Iterate through implementations of this algfamily. For each impl,
        copy the implementation-specific set of source files. Modify
        self.implementations[impl_key].sources to take precise inventory of
        source files.
        """
        for impl_key, impl_meta in self.implementations.items():
            impl_meta.copy_sources_sets(
                self.algtype, algfamily_key, impl_key, upstreams, excludes, has_jasmin
            )


class OQSMeta:
    def __init__(
        self,
        schema_version: int,
        jasminc_version: str,
        upstreams: dict[str, UpstreamMeta],
        algfamilies: dict[str, AlgFamilyMeta],
    ):
        self.schema_version = schema_version
        self.jasminc_version = jasminc_version
        self.upstreams = upstreams
        self.algfamilies = algfamilies

    def has_jasmin(self):
        """Return True iff calling `jasminc -version` returns the version
        specified in self.jasminc_version
        """
        try:
            proc = subprocess.run(
                ["jasminc", "-version"], capture_output=True, text=True, check=True
            )
            # Expected: "Jasmin Compiler 2023.06.3"
            return self.jasminc_version == proc.stdout.strip().split()[-1]
        except FileNotFoundError:
            logger.warning("jasminc not found")
            return False
        except subprocess.CalledProcessError as e:
            logger.error("jasminc failed with exit code %s", e.returncode)
            raise e
        except Exception as e:
            logger.error(e)
            raise e

    def check_foreign_keys(self):
        """Raise exception if foreign keys are invalid"""
        warnings.warn("Foreign key checks not yet implemented")

    @staticmethod
    def from_dict(builderconfig: OQSBuilderConfig, raw_oqs_meta: dict):
        if not OQS_META_DATOR.validate(raw_oqs_meta):
            raise ValueError(f"{builderconfig.oqs_meta_path} failed schema validation")
        schema_version = raw_oqs_meta["schema_version"]
        jasminc_version = raw_oqs_meta["jasminc_version"]
        upstreams = {
            key: UpstreamMeta.from_dict(builderconfig.patch_dir, upstream_meta)
            for key, upstream_meta in raw_oqs_meta["upstreams"].items()
        }
        algfamilies = {
            key: AlgFamilyMeta.from_dict(family_meta)
            for key, family_meta in raw_oqs_meta["algfamilies"].items()
        }
        oqs_meta = OQSMeta(schema_version, jasminc_version, upstreams, algfamilies)
        oqs_meta.check_foreign_keys()
        return oqs_meta


def clone_upstreams(
    upstreams_dir: str, upstreams: dict[str, UpstreamMeta], has_jasmin: bool
):
    """
    :param has_jasmin: if False, do not clone libjade. Note that if builder is
        configured to hard fail without jasminc, the hard fail would have
        happened before cloning upstreams, so it is safe to assume "ignore"
        instead of hard failing
    """
    for upstream_key, upstream in upstreams.items():
        if (not has_jasmin) and upstream_key == "libjade":
            logger.warning("libjade is not cloned because jasminc is not present")
            continue
        logger.info("Cloning %s", upstream_key)
        upstream.clone_and_patch(upstream_key, upstreams_dir)


def copy_sources(
    upstreams: dict[str, UpstreamMeta],
    families: dict[str, AlgFamilyMeta],
    excludes: list[str],
    has_jasmin: bool,
):
    """Copy files from upstreams into appropriate destinations.

    There are two categories of copies. Family-level common source files are
    declared as <common_src_key>:<common_src_meta> pairs under each
    family meta. Implementation-level source files are declared as unnamed
    <impl_src_meta> under <impl_meta.sources>. Family-level common
    sources will be copied into
    src/<algtype>/<family_key>/<common_src_key>. Impl-level sources
    will be copied into
    src/<algtype>/<family_key>/<impl_key>.
    """

    for algfamily_key, algfamily_meta in families.items():
        algfamily_meta.copy_common_src(algfamily_key, upstreams, excludes)
        algfamily_meta.copy_implementations_src(
            algfamily_key, upstreams, excludes, has_jasmin
        )


def render_sources(
    algfamily_key: str,
    algfamily_meta: AlgFamilyMeta,
    builderconfig: OQSBuilderConfig,
    dryrun: bool = False,
):
    """For each parameter set, generate one source file implementing OQS public
    API for this parameter set. The source file will be placed at:
    <liboqs_dir>/src/<algtype>/<family_key>/<algtype>_<parameter_key>.c

    :param dryrun: if True, print the rendered source file to stdout instead of
    writing to the actual file
    """
    if not algfamily_meta.implementations:
        logger.warning(
            "%s is not an upstream integration. Skiping render_sources", algfamily_meta
        )
        return

    for param_key, param_meta in algfamily_meta.parameters.items():
        oqsapi_src_filename = f"{algfamily_meta.algtype.value}_{param_key}.c"
        oqsapi_src_full_path = os.path.join(
            algfamily_meta.algtype.dir, algfamily_key, oqsapi_src_filename
        )
        if algfamily_meta.algtype == AlgTypes.Kem:
            template_path = os.path.join(
                builderconfig.templates_dir, builderconfig.kem_src_template_filename
            )
        elif algfamily_meta.algtype == AlgTypes.Sig:
            template_path = os.path.join(
                builderconfig.templates_dir, builderconfig.sig_src_template_filename
            )
            warnings.warn("External Mu omitted for now")
        else:
            raise ValueError(f"Invalid alg type {algfamily_meta.algtype}")

        impls = {
            impl_key: impl_meta
            for impl_key, impl_meta in algfamily_meta.implementations.items()
            if impl_meta.parameter == param_key
        }
        assert param_meta.default_impl
        default_impl_meta = impls[param_meta.default_impl]
        addtl_impls = {k: m for k, m in impls.items() if k != param_meta.default_impl}
        addtl_impls_w_keypair_derand = {
            k: m
            for k, m in addtl_impls.items()
            if (isinstance(m.api, KemApiMeta) and m.api.signature_keypair_derand)
        }
        addtl_impls_w_enc_derand = {
            k: m
            for k, m in addtl_impls.items()
            if (isinstance(m.api, KemApiMeta) and m.api.signature_enc_derand)
        }

        with open(template_path, "r") as template_f:
            template = jinja2.Template(template_f.read())
        rendered = template.render(
            {
                "algfamily_key": algfamily_key,
                "algfamily_meta": algfamily_meta,
                "param_key": param_key,
                "param_meta": param_meta,
                "impls": impls,
                "default_impl_meta": default_impl_meta,
                "addtl_impls": addtl_impls,
                "addtl_impls_w_keypair_derand": addtl_impls_w_keypair_derand,
                "addtl_impls_w_enc_derand": addtl_impls_w_enc_derand,
            }
        )
        if dryrun:
            print(rendered)
            continue
        with open(oqsapi_src_full_path, "w") as f:
            f.write(rendered)
        # FIX: take inventory of OQS API source files


def render_header(
    algfamily_key: str,
    algfamily_meta: AlgFamilyMeta,
    builderconfig: OQSBuilderConfig,
    dryrun: bool = False,
):
    """The header file lists cryptographic parameters and declares public API's"""
    if not algfamily_meta.implementations:
        logger.warning(
            "%s is not an upstream integration. Skiping render_header", algfamily_meta
        )
        return

    oqsapi_header_filename = f"{algfamily_meta.algtype.value}_{algfamily_key}.h"
    oqsapi_header_path = os.path.join(
        algfamily_meta.algtype.dir, algfamily_key, oqsapi_header_filename
    )
    if algfamily_meta.algtype == AlgTypes.Kem:
        template_filename = builderconfig.kem_header_template_filename
    elif algfamily_meta.algtype == AlgTypes.Sig:
        template_filename = builderconfig.sig_header_template_filename
    else:
        raise ValueError(f"Invalid alg type {algfamily_meta.algtype}")

    template_path = os.path.join(builderconfig.templates_dir, template_filename)
    with open(template_path, "r") as template_f:
        template = jinja2.Template(template_f.read())
    rendered = template.render(
        {
            "algfamily_key": algfamily_key,
            "algfamily_meta": algfamily_meta,
        }
    )
    if dryrun:
        print(rendered)
        return
    with open(oqsapi_header_path, "w") as f:
        f.write(rendered)
        # FIX: take inventory of OQS API header files


def render_build_file(
    algfamily_key: str,
    algfamily_meta: AlgFamilyMeta,
    builderconfig: OQSBuilderConfig,
    dryrun: bool = False,
):
    """Generate the cmake list file for one algorithm family"""
    if not algfamily_meta.implementations:
        logger.warning(
            "%s is not copied from upstream. CMake list file will not be refreshed",
            algfamily_key,
        )
        return

    listfile_path = os.path.join(
        algfamily_meta.algtype.dir, algfamily_key, "CMakeLists.txt"
    )
    template_path = os.path.join(
        builderconfig.templates_dir, builderconfig.listfile_template_filename
    )
    with open(template_path) as f:
        template = jinja2.Template(f.read())
    rendered = template.render(
        {
            "algfamily_key": algfamily_key,
            "algfamily_meta": algfamily_meta,
        }
    )

    if dryrun:
        print(rendered)
        return
    with open(listfile_path, "w") as f:
        f.write(rendered)


def render_documentation():
    warnings.warn("docs/algorithms rendering not implemented yet")


def render_source_build_docs(
    algfamilies: dict[str, AlgFamilyMeta],
    builderconfig: OQSBuilderConfig,
    dryrun: bool = False,
):
    """Generate source and header files that implement OQS public API"""
    for algfamily_key, algfamily_meta in algfamilies.items():
        render_sources(algfamily_key, algfamily_meta, builderconfig, dryrun)
        render_header(algfamily_key, algfamily_meta, builderconfig, dryrun)
        render_build_file(algfamily_key, algfamily_meta, builderconfig, dryrun)
        render_documentation()


def make_parser():
    parser = argparse.ArgumentParser(
        "oqsbuilder", description="Utilities for building liboqs"
    )
    parser.add_argument("--oqs-meta", type=str, help="Path to OQS_META.yml")
    parser.add_argument(
        "--log-level",
        choices=CLI_ARGS_LOGLEVEL_COICES,
        default="info",
        help="Only record log messages at or above the specified level",
    )
    subparsers = parser.add_subparsers(title="subcommand", required=True)
    copy_subparser = subparsers.add_parser("copy")
    copy_subparser.add_argument(
        "--keep-upstreams",
        action="store_true",
        help="Do not delete upstream repositories",
    )
    copy_subparser.add_argument(
        "--upstreams-dir",
        type=str,
        help=(
            "Clone the upstream repository to the specified directory. "
            "If the repository is already cloned, do nothing. "
            "Repositories cloned into this directory will not be deleted. "
            "If not specified, upstream repos are cloned into a temporary "
            "subdirectory under $LIBOQS_DIR."
        ),
    )
    copy_subparser.set_defaults(func=copy_from_upstreams)
    return parser


def copy_from_upstreams(builderconfig: OQSBuilderConfig, oqs_meta: OQSMeta):
    """The main executable for the copy subcommand"""
    if builderconfig.upstreams_cached:
        # if upstreams are cached, then builderconfig.upstreams_dir must have been
        # overwritten from the command line argument --upstreams-dir
        clone_upstreams(
            builderconfig.upstreams_dir, oqs_meta.upstreams, oqs_meta.has_jasmin()
        )
        copy_sources(
            oqs_meta.upstreams,
            oqs_meta.algfamilies,
            builderconfig.never_copy,
            oqs_meta.has_jasmin(),
        )
        return

    with TemporaryDirectory(
        dir=builderconfig.upstreams_dir,
        prefix="_upstreams_",
        delete=builderconfig.delete_upstreams,
    ) as upstreams_dir:
        logger.info("Cloning repositories into %s", upstreams_dir)
        clone_upstreams(
            upstreams_dir,
            oqs_meta.upstreams,
            oqs_meta.has_jasmin(),
        )
        copy_sources(
            oqs_meta.upstreams,
            oqs_meta.algfamilies,
            builderconfig.never_copy,
            oqs_meta.has_jasmin(),
        )


def main():
    cli_args = make_parser().parse_args()
    logger.setLevel(cli_loglevel_choices_to_loglevel(cli_args.log_level))
    builderconfig = OQSBuilderConfig()
    builderconfig.overwrite_with_cli_args(cli_args)

    with open(builderconfig.oqs_meta_path) as f:
        oqs_meta = yaml.safe_load(f)
    for demo_algfamily in builderconfig.demo_algfamilies:
        demo_algfamily_meta = oqs_meta["algfamilies"].pop(demo_algfamily)
        if demo_algfamily_meta:
            logger.info("Removed demo alg family %s", demo_algfamily)

    oqs_meta = OQSMeta.from_dict(builderconfig, oqs_meta)

    if (not oqs_meta.has_jasmin()) and builderconfig.fail_on_jasminc:
        raise FileNotFoundError("jasminc not found, exiting")

    cli_args.func(builderconfig, oqs_meta)


if __name__ == "__main__":
    main()
