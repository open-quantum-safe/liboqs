#!/usr/bin/env python3

# TODO: support remote config

import abc
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
DEFAULT_OQS_META_PATH = os.path.join(LIBOQS_DIR, "OQS_META.yml")
DEFAULT_PATCH_DIR = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
DEFAULT_NEVER_COPY = [
    "Makefile.Microsoft_nmake",
    "Makefile",
]


class OQSBuilderConfig:
    def __init__(
        self,
        oqs_meta_path: str | None = None,
        upstreams_dir: str | None = None,
        patch_dir: str | None = None,
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
        self.oqs_meta_path = oqs_meta_path or DEFAULT_OQS_META_PATH
        self.upstreams_dir = upstreams_dir or LIBOQS_DIR
        self.patch_dir = patch_dir or DEFAULT_PATCH_DIR
        self.delete_upstreams = True
        self.demo_algfamilies = ["demo_alg"]
        self.fail_on_jasminc = fail_on_jasminc
        self.never_copy = never_copy


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
    "link_libs": (Fields.OPTIONAL, Fields.Array(Fields.Text(), allowempty=False)),
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


class RelationalChecks:
    @staticmethod
    def impl_key_exists(family_meta: dict) -> bool:
        impl_keys = family_meta.get("implementations", {}).keys()
        for param_key, param_meta in family_meta.get("parameters", {}):
            default_impl_key = param_meta.get("default-implementation", None)
            if default_impl_key and (default_impl_key not in impl_keys):
                logger.error("%s default impl %s invalid", param_key, default_impl_key)
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
    def run_all(oqs_meta: dict) -> bool:
        raise NotImplementedError()


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


def check_jasminc(requirement: str) -> bool:
    try:
        proc = subprocess.run(
            ["jasminc", "-version"], capture_output=True, text=True, check=True
        )
        return requirement == proc.stdout.strip().split()[-1]
    except FileNotFoundError:
        logger.warning("jasminc not found")
        return False
    except subprocess.CalledProcessError as e:
        logger.error("jasminc failed with exit code %s", e.returncode)
        raise e
    except Exception as e:
        logger.error(e)
        raise e


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
        os.mkdir(upstream_dir)

        run_subprocess(["git", "init"], upstream_dir)
        run_subprocess(["git", "remote", "add", "origin", self.git_url], upstream_dir)
        run_subprocess(
            ["git", "fetch", "--depth=1", "origin", self.git_commit], upstream_dir
        )
        run_subprocess(["git", "checkout", "--detach", "FETCH_HEAD"], upstream_dir)

        self._dir = upstream_dir

        if self.patch_full_paths:
            run_subprocess(
                ["git", "apply", "--whitespace=fix"] + self.patch_full_paths, self._dir
            )

        if not self.post_patches:
            return
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
        self.default_impl = default_impl
        self.memopt_impl = memopt_impl

    @staticmethod
    def from_dict(meta: dict):
        strength = SecStrength(meta["security-strength"])
        model = meta["security-model"]
        pubkeylen = meta["length-public-key"]
        privkeylen = meta["length-secret-key"]
        siglen = meta["length-signature"]
        mulen = meta["length-mu"]
        nistkat = meta.get("nistkat-sha256", None)
        enable_by = meta["enable_by"]
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
            default_impl,
            memopt_impl,
        )


class CommonSrcMeta:
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
        self, algtype: AlgTypes, algfamily_key: str, common_key: str, paths: list[str]
    ):
        """Set self._files to the precise list of files in this set of
        family-level common source files. The paths are relative to
        $LIBOQS_DIR/src/<algtype>/<algfamily>/<common_key>
        """
        base_dir = os.path.join(
            LIBOQS_DIR, "src", algtype.value, algfamily_key, common_key
        )
        for path in paths:
            fullpath = os.path.join(base_dir, path)
            if not os.path.isfile(fullpath):
                raise FileNotFoundError(f"{fullpath} not found")
        self._paths = paths

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

    def copy_files(
        self,
        algtype: AlgTypes,
        algfamily_key: str,
        dest_dirname: str,
        upstream: UpstreamMeta,
        excludes: list[str],
    ):
        """Copy one set of source files from upstream to destination directory

        :param dest_dirname: directory name under src/algtype/family
        """
        # source_paths are relative to <upstream._dir>/<self.base_dir> and may
        # contain subdirectories
        source_paths = upstream.match_path_patterns(
            self.upstream_key, self.patterns, self.base_dir, excludes
        )

        destdir = os.path.join(
            LIBOQS_DIR, "src", algtype.value, algfamily_key, dest_dirname
        )
        # destpaths should be relative to:
        # $LIBOQS_DIR/src/<algtype>/<algfamily>/<dest_dirname>
        destpaths: list[str] = []
        for source_path in source_paths:
            source_full_path = os.path.join(
                upstream.dir(self.upstream_key), self.base_dir or "", source_path
            )
            if os.path.isfile(source_full_path):
                _, filename = os.path.split(source_full_path)
                destpath = (
                    os.path.join(destdir, source_path)
                    if self.preserve_subdirs
                    else os.path.join(destdir, filename)
                )
                destpath_dir, _ = os.path.split(destpath)
                if not os.path.isdir(destpath_dir):
                    os.makedirs(destpath_dir)
                logger.info(
                    "Copy file %s into %s",
                    pathlib.Path(source_full_path).relative_to(
                        pathlib.Path(LIBOQS_DIR)
                    ),
                    pathlib.Path(destpath).relative_to(pathlib.Path(LIBOQS_DIR)),
                )
                shutil.copy2(source_full_path, destpath)
                destpaths.append(source_path if self.preserve_subdirs else filename)
            elif os.path.isdir(source_full_path):
                raise NotImplementedError("""OQSBuilder currently does not
                support recursively copying directories from upstream. Use
                explicit set of files instead.""")
            else:
                raise ValueError(f"{source_full_path} is invalid")
        self.set_paths(algtype, algfamily_key, dest_dirname, destpaths)

    def set_paths(
        self,
        algtype: AlgTypes,
        algfamily_key: str,
        impl_destdirname: str,
        paths: list[str],
    ):
        """
        :param paths: a list of paths relative to
        $LIBOQS_DIR/src/<algtype>/<algfamily>/<impl_destdirname>
        """
        base_dir = os.path.join(
            LIBOQS_DIR, "src", algtype.value, algfamily_key, impl_destdirname
        )
        for path in paths:
            fullpath = os.path.join(base_dir, path)
            if not os.path.isfile(fullpath):
                raise FileNotFoundError(f"{fullpath} not found")
        self._paths = paths

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
        value = meta["name"]
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
        link_libs: list[str],
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
        is_libjade_impl = any(
            [
                isinstance(impl_src, ImplSrcMeta) and impl_src.upstream_key == "libjade"
                for impl_src in self.sources
            ]
        )
        if is_libjade_impl and (not has_jasmin):
            logger.warning("Impl %s is skipped due to missing jasminc", impl_key)
            return
        for impl_src in self.sources:
            if isinstance(impl_src, CommonSrcRef):
                continue
            upstream = upstreams[impl_src.upstream_key]
            impl_src.copy_files(
                algtype, algfamily_key, self.subdirname or impl_key, upstream, excludes
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
        libs = meta.get("link_libs", [])
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
            libs,
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
        if self.common_src:
            raise NotImplementedError("TODO: will implement when MQOM is filled in")

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


class OqsMeta:
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

    @staticmethod
    def from_dict(builderconfig: OQSBuilderConfig, meta: dict):
        schema_version = meta["schema_version"]
        jasminc_version = meta["jasminc_version"]
        upstreams = {
            key: UpstreamMeta.from_dict(builderconfig.patch_dir, upstream_meta)
            for key, upstream_meta in meta["upstreams"].items()
        }
        algfamilies = {
            key: AlgFamilyMeta.from_dict(family_meta)
            for key, family_meta in meta["algfamilies"].items()
        }
        return OqsMeta(schema_version, jasminc_version, upstreams, algfamilies)


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
            logger.info("libjade is not cloned because jasminc is not present")
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


if __name__ == "__main__":
    builderconfig = OQSBuilderConfig()

    with open(builderconfig.oqs_meta_path) as f:
        oqs_meta = yaml.safe_load(f)
    for demo_algfamily in builderconfig.demo_algfamilies:
        demo_algfamily_meta = oqs_meta["algfamilies"].pop(demo_algfamily)
        if demo_algfamily_meta:
            logger.info("Removed demo alg %s", demo_algfamily)

    if not OQS_META_DATOR.validate(oqs_meta):
        logger.error("%s failed schema validation", builderconfig.oqs_meta_path)
    # TODO: implement relational check
    # if not RelationalChecks.run_all(oqs_meta):
    #     logger.error("%s failed relational checks", builderconfig.oqs_meta_path)

    oqs_meta = OqsMeta.from_dict(builderconfig, oqs_meta)

    # TODO: where to issue warnings and errors? The delegation is not clear.
    has_jasmin = check_jasminc(oqs_meta.jasminc_version)
    if (not has_jasmin) and builderconfig.fail_on_jasminc:
        logger.error("jasminc not found, exiting")

    with TemporaryDirectory(
        dir=builderconfig.upstreams_dir,
        prefix="_upstreams_",
        delete=builderconfig.delete_upstreams,
    ) as upstreams_dir:
        logger.info("Cloning repositories into %s", upstreams_dir)
        clone_upstreams(
            upstreams_dir,
            oqs_meta.upstreams,
            has_jasmin,
        )
        # TODO: fill in remote metadata

        copy_sources(
            oqs_meta.upstreams,
            oqs_meta.algfamilies,
            builderconfig.never_copy,
            has_jasmin,
        )
