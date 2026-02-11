#!/usr/bin/env python3
"""OQSBuilder

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- ✅ move source file from upstream into `liboqs/src`
- 🔨 Re-produce existing builds
    - ✅ ML-KEM
        - ✅ render family-level `CMakeLists.txt`
        - ✅ render family-level header file (e.g. `kem_ml_kem.h`)
        - ✅ render family-level source file (e.g. `kem_ml_kem_512.c`)
        - 🔨 render `.CMake/alg_support.cmake`
        - 🔨 render documentation
- ✅ Parse `oqsbuildfile.yml` into structured data instead of Python dictionary
- 🔨 Experiment with converting `oqsbuildfile.yml` into TOML. TOML is better
  because I can key a table with dot-separated namespacing instead of indenting
  by more than 3 levels
- 🔨 `copies` should support direct mapping, re-usable mapping in the same
  oqsbuildfile, and remote mapping from `META.yaml`
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`
"""

import argparse
import enum
import os
import shutil
import subprocess
import sys
import inspect
from tempfile import TemporaryDirectory
import typing

import jinja2
import yaml

__version__ = "0.1"
_liboqs_dir = os.environ.get("LIBOQS_DIR")
if not _liboqs_dir:
    raise KeyError("Missing environment variable LIBOQS_DIR")
LIBOQS_DIR = _liboqs_dir
LIBOQS_PATCH_DIR = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
LIBOQS_SRC_DIR = os.path.join(LIBOQS_DIR, "src")
LIBOQS_JINJA_TEMPLATES_DIR = os.path.join(LIBOQS_DIR, "scripts", "oqsbuilder_templates")

OQSBUILDER_AUTOFORMAT_CMAKE = False
try:
    import gersemi as _

    OQSBUILDER_AUTOFORMAT_CMAKE = True
except:
    pass
OQSBUILDFILE_PATH = os.path.join(LIBOQS_DIR, "scripts", "oqsbuildfile.yml")

# NOTE: typing.NewType is strict typing, so LocalCopiesKey is not ImplKey even though
#   they are both str.
LiteralCopiesMap = typing.NewType("LiteralCopiesMap", dict[str, str])
LocalCopiesKey = typing.NewType("LocalCopiesKey", str)
ImplKey = typing.NewType("ImplKey", str)

SRC_FILE_EXTS = (".c", ".s", ".S", ".cpp", ".cu")

KEM_CMAKE_TEMPLATE = "kem_cmakelists.txt.jinja"
KEM_SRC_TEMPLATE = "kem_src.c.jinja"
KEM_HEADER_TEMPLATE = "oqs_kem.h.jinja"


class CryptoPrimitive(enum.Enum):
    KEM = 1
    SIG = 2
    STFL_SIG = 3

    def get_oqsbuildfile_key(self) -> str:
        match self:
            case CryptoPrimitive.KEM:
                return "kems"
            case CryptoPrimitive.SIG:
                return "sigs"
            case CryptoPrimitive.STFL_SIG:
                return "stfl_sigs"

    def get_subdirectory_name(self) -> str:
        # TODO: consider refactoring src/ so it matches the plural cases
        match self:
            case CryptoPrimitive.KEM:
                return "kem"
            case CryptoPrimitive.SIG:
                return "sig"
            case CryptoPrimitive.STFL_SIG:
                return "stfl_sig"


class NistLevel(enum.Enum):
    """https://csrc.nist.gov/projects/post-quantum-cryptography/post-quantum-cryptography-standardization/evaluation-criteria/security-(evaluation-criteria)"""

    LEVEL_1 = 1
    LEVEL_2 = 2
    LEVEL_3 = 3
    LEVEL_4 = 4
    LEVEL_5 = 5


class KemSecurity(enum.Enum):
    IND_CPA = 1
    IND_CCA = 2


class ParameterSet:
    # NOTE: a parameter set is associated with a family. Should this class include
    #   the family key?
    def __init__(
        self,
        key: str,
        name: str,
        pklen: int,
        sklen: int,
        ctlen: int,
        sslen: int,
        keypair_seedlen: int,
        encap_seedlen: int,
        enable_by: str,
        default_impl_key: str,
        nist_level: NistLevel,
        ind_cca: bool,
    ):
        self.key = key
        """Unique identifier of parameter set, such as ml_kem_512"""

        self.name = name
        """Human-friendly name of parameter set, such as \"ML-KEM-512\""""

        self.pklen = pklen
        """Length of encapsulation key in bytes"""

        self.sklen = sklen
        """Length of decapsulation key in bytes"""

        self.ctlen = ctlen
        """Length of ciphertext in bytes"""

        self.sslen = sslen
        """Length of shared secret in bytes"""

        self.keypair_seedlen = keypair_seedlen
        """Length of seed for deterministic key generation"""

        self.encap_seedlen = encap_seedlen
        """Length of seed for deterministic encapsulation"""

        self.enable_by = enable_by
        """A C pre-processing macro that enables this parameter set.

        If a parameter set if disabled, then all implementations of this parameter
        set are disabled"""

        self.default_impl_key = default_impl_key
        """The key of the default implementation

        The default implementation's "enable_by" field will be overwritten so that
        if a parameter set is enabled, then its default implementation is guaranteed
        to be enabled.

        If a family supports some optional API (e.g. de-randomized keypair for KEM/SIG,
        and de-randomized encapsulation for KEM), then the default implementation
        must support all of these optional API."""

        self.nist_level = nist_level
        """The security level of this parameter set"""

        # TODO: security should not be an attribute of a parameter set, but an attribute
        # of a family?
        self.ind_cca = ind_cca
        """True iff this parameter set achieves IND-CCA security"""

        # TODO: does this need parameterization?
        self.api_src = f"{CryptoPrimitive.KEM.get_subdirectory_name()}_{key}.c"
        """Name of the source file that contains the OQS common API

        For example, "kem_ml_kem_512.c" contains implementation of OQS function
        OQS_KEM_ml_kem_512_new. Defaults to <kem|sig|stfl_sig>_<param_key>.c
        """

    @staticmethod
    def from_oqsbuildfile(key: str, param_meta: dict):
        return ParameterSet(
            key,
            param_meta["name"],
            param_meta["pklen"],
            param_meta["sklen"],
            param_meta["ctlen"],
            param_meta["sslen"],
            param_meta.get("keypair_seedlen", 0),
            param_meta.get("encap_seedlen", 0),
            param_meta["enable_by"],
            param_meta["default_impl"],
            param_meta["nist_level"],
            param_meta["ind_cca"],
        )


class Architecture(enum.Enum):
    PORTABLE = 1
    X86_64 = 2
    AARCH64 = 3
    CUDA = 4
    ICICLE_CUDA = 5

    def enable_by(self) -> str | None:
        match self:
            case Architecture.CUDA:
                return "OQS_USE_CUPQC"
            case Architecture.ICICLE_CUDA:
                return "OQS_USE_ICICLE"
            case _:
                return None

    @staticmethod
    def from_oqsbuildfile(arch_key: str):
        match arch_key:
            case "portable":
                return Architecture.PORTABLE
            case "x86_64":
                return Architecture.X86_64
            case "aarch64":
                return Architecture.AARCH64
            case "cuda":
                return Architecture.CUDA
            case "icicle_cuda":
                return Architecture.ICICLE_CUDA
        raise KeyError(f"Invalid architecture {arch_key}")


class RuntimeCpuFeature(enum.Enum):
    AVX2 = enum.auto()
    BMI2 = enum.auto()
    POPCNT = enum.auto()
    ASIMD = enum.auto()

    @staticmethod
    def from_oqsbuildfile(feature: str):
        match feature:
            case "avx2":
                return RuntimeCpuFeature.AVX2
            case "bmi2":
                return RuntimeCpuFeature.BMI2
            case "popcnt":
                return RuntimeCpuFeature.POPCNT
            case "asimd":
                return RuntimeCpuFeature.ASIMD
        raise KeyError(f"Invalid CPU feature {feature}")

    def to_source_code(self) -> str:
        match self:
            case RuntimeCpuFeature.AVX2:
                return "avx2"
            case RuntimeCpuFeature.BMI2:
                return "bmi2"
            case RuntimeCpuFeature.POPCNT:
                return "popcnt"
            case RuntimeCpuFeature.ASIMD:
                return "arm_neon"


class CmakeScope(enum.Enum):
    PUBLIC = enum.auto()
    PRIVATE = enum.auto()
    INTERFACE = enum.auto()

    def to_cmake_scope(self):
        match self:
            case CmakeScope.PUBLIC:
                return "PUBLIC"
            case CmakeScope.PRIVATE:
                return "PRIVATE"
            case CmakeScope.INTERFACE:
                return "INTERFACE"

    @staticmethod
    def from_oqsbuildfile(key: str):
        match key:
            case "public":
                return CmakeScope.PUBLIC
            case "private":
                return CmakeScope.PRIVATE
            case "interface":
                return CmakeScope.INTERFACE
        return KeyError(f"invalid cmake scope {key}")


class CmakeInclude:
    def __init__(self, scope: CmakeScope, value: str):
        self.scope = scope
        self.value = value
        """Currently we only support string literals, no templating. Whatever is
        written in the oqsbuildfile will be directly copied into the CMake list
        file. Using CMake variables is recommended"""

    def __repr__(self):
        return f"<{self.__class__.__name__} {self.scope.to_cmake_scope()} {self.value}>"


class CmakeCompileOpt:
    def __init__(self, scope: CmakeScope, value: str):
        self.scope = scope
        self.value = value
        """Currently we only support string literals, no templating. Whatever is
        written in the oqsbuildfile will be directly copied into the CMake list
        file. Using CMake variables is recommended"""

    def __repr__(self):
        return f"<{self.__class__.__name__} {self.scope.to_cmake_scope()} {self.value}>"


class CmakeLinkLib:
    def __init__(self, scope: CmakeScope, value: str):
        self.scope = scope
        self.value = value
        """Currently we only support string literals, no templating. Whatever is
        written in the oqsbuildfile will be directly copied into the CMake list
        file. Using CMake variables is recommended"""

    def __repr__(self):
        return f"<{self.__class__.__name__} {self.scope.to_cmake_scope()} {self.value}>"


class KemApi:
    def __init__(
        self,
        keypair: str,
        encaps: str,
        decaps: str,
        keypair_derand: str | None,
        encaps_derand: str | None,
    ):
        self.keypair = keypair
        self.encaps = encaps
        self.decaps = decaps
        self.keypair_derand = keypair_derand
        self.encaps_derand = encaps_derand


class Implementation:
    # NOTE: an impl is associated with a parameter set. Should this class include
    #   the param key?
    """A collection of source files and compilation instructions that implement
    a specific parameter set
    """

    def __init__(
        self,
        key: str,
        upstream_key: str,
        param_key: str,
        copies: LiteralCopiesMap,
        enable_by: str,
        arch: Architecture,
        cuda_arch: str | None,
        runtime_cpu_features: list[RuntimeCpuFeature],
        includes: list[CmakeInclude],
        compile_opts: list[CmakeCompileOpt],
        link_libs: list[CmakeLinkLib],
        api_names: KemApi,
    ):
        self.key = key
        """The unique identifier of this implementation
        Example: mlkem-native_ml-kem-1024_x86_64"""

        self.upstream_key = upstream_key
        """The upstream repository from which source files are copies"""

        self.param_key = param_key
        """The parameter set implemented"""

        self.copies = copies
        """Instuction for how to copy source files

        In oqsbuildfile, an implementation's "copies" field can be one of three
        types:
        - (literal) A literal map from destinations to sources. Each destination
            path is relative to the implementation sub-directory. Each source
            path is relative to the root of upstream repository.
        - (local) A key referencing a re-usable literal map under the "copies" section
        - (remote) A URL referencing a remote META.yml file (legacy)
        At parsing, local and remote type will be converte into literal mappings,
        so here the "copies" attribute will always be a literal mapping.
        """

        self.enable_by = enable_by
        """C pre-processing macro that controls whether this implementation is
        enabled"""

        self.arch = arch
        """The hardware architecture or micro-architecture of this implementation"""

        self.cuda_arch = cuda_arch
        """Will be passed into set_property(... CUDA_ARCHITECTURES <cuda_arch>)

        https://cmake.org/cmake/help/latest/prop_tgt/CUDA_ARCHITECTURES.html
        """

        self.runtime_cpu_features = runtime_cpu_features
        self.includes = includes
        self.compile_opts = compile_opts
        self.link_libs = link_libs
        # print("includes: ", self.includes)
        # print("compile_opts: ", self.compile_opts)
        # print("link_libs: ", self.link_libs)
        self.api_names = api_names
        self.cmake_src_paths = [
            os.path.join("${IMPL_KEY}", path)
            for path in copies.keys()
            if os.path.splitext(path)[1] in SRC_FILE_EXTS
        ]
        """A list of source file paths"""

    def generate_cmake_add_library(self) -> str:
        """Generate an "add_library" statement

        Example: add_library(<impl_key> OBJECT <src1> <src2>)
        """
        src_paths_str = " ".join(self.cmake_src_paths)
        return f"add_library({self.key} OBJECT {src_paths_str})"

    def generate_cmake_includes(self, scope: CmakeScope) -> str:
        """Generate a target_include_directories statement

        target_include_directories(<impl_key> <SCOPE> <value1> <value2>)
        """
        includes = [
            include.value for include in self.includes if include.scope == scope
        ]
        args = " ".join(includes)
        if not includes:
            return ""
        return f"target_include_directories({self.key} {scope.to_cmake_scope()} {args})"

    def generate_cmake_compile_opts(self, scope: CmakeScope) -> str:
        """Generate a target_compile_options statement

        target_compile_options(<impl_key> <SCOPE> <value1> <value2>)
        """
        compile_opts = [
            compile_opt.value
            for compile_opt in self.compile_opts
            if compile_opt.scope == scope
        ]
        args = " ".join(compile_opts)
        if not compile_opts:
            return ""
        return f"target_compile_options({self.key} {scope.to_cmake_scope()} {args})"

    def generate_cmake_link_libs(self, scope: CmakeScope) -> str:
        """Generate a target_link_lib_directories statement

        target_link_libraries(<impl_key> <SCOPE> <value1> <value2>)
        """
        link_libs = [
            link_lib.value for link_lib in self.link_libs if link_lib.scope == scope
        ]
        args = " ".join(link_libs)
        if not link_libs:
            return ""
        return f"target_link_libraries({self.key} {scope.to_cmake_scope()} {args})"

    @staticmethod
    def from_oqsbuildfile(key: str, impl_meta: dict):
        runtime_cpu_features = [
            RuntimeCpuFeature.from_oqsbuildfile(feat)
            for feat in impl_meta.get("runtime_cpu_features", [])
        ]
        includes = []
        if oqsbuildfile_includes := impl_meta.get("includes", {}):
            includes += [
                CmakeInclude(CmakeScope.PRIVATE, val)
                for val in oqsbuildfile_includes.get("private", [])
            ]
            includes += [
                CmakeInclude(CmakeScope.PUBLIC, val)
                for val in oqsbuildfile_includes.get("public", [])
            ]
        compile_opts = []
        if oqsbuildfile_compile_opts := impl_meta.get("compile_opts", {}):
            compile_opts += [
                CmakeCompileOpt(CmakeScope.PRIVATE, val)
                for val in oqsbuildfile_compile_opts.get("private", [])
            ]
            compile_opts += [
                CmakeCompileOpt(CmakeScope.PUBLIC, val)
                for val in oqsbuildfile_compile_opts.get("public", [])
            ]
        link_libs = []
        if oqsbuildfile_link_libs := impl_meta.get("link_libs", {}):
            link_libs += [
                CmakeLinkLib(CmakeScope.PRIVATE, val)
                for val in oqsbuildfile_link_libs.get("private", [])
            ]
            link_libs += [
                CmakeLinkLib(CmakeScope.PUBLIC, val)
                for val in oqsbuildfile_link_libs.get("public", [])
            ]
        api_names = KemApi(
            impl_meta["keypair"],
            impl_meta["enc"],
            impl_meta["dec"],
            impl_meta.get("keypair_derand", None),
            impl_meta.get("enc_derand", None),
        )
        return Implementation(
            key,
            impl_meta["upstream"],
            impl_meta["param"],
            impl_meta["copies"],
            impl_meta["enable_by"],
            Architecture.from_oqsbuildfile(impl_meta["arch"]),
            impl_meta.get("cuda_arch", None),
            runtime_cpu_features,
            includes,
            compile_opts,
            link_libs,
            api_names,
        )


# TODO: consider making KemFamily inherit from "AlgorithmFamily"
class KemFamily:
    """A family refers to a broad category of cryptographic algorithm based on a
    shared mathematical foundation. For example, ML-KEM is a family of KEM algorithms
    based on module-lattice constructions.
    """

    def __init__(
        self,
        key: str,
        version: str,
        header: str | None,
        derandomized_keypair: bool,
        derandomized_encaps: bool,
        params: dict[str, ParameterSet],
        impls: dict[str, Implementation],
    ):
        self.key = key
        """unique identifier of the family, such as \"ml_kem\""""

        self.primitive = CryptoPrimitive.KEM
        """the type of primitive"""

        self.header = header or f"{self.primitive.get_subdirectory_name()}_{key}.h"
        """name of the family-level header file
        Family-level header file parameter values and function declarations.
        Defaults to <kem|sig|sig_stfl>_<family_key>.h"""

        self.version = version
        """liboqs will integrate one version per family.
        If there is a case to support multiple versions of the same family,
        such as supporting Kyber round 2 and Kyber round 3 at the same time,
        they should be separate families"""

        self.derandomized_keypair = derandomized_keypair
        """True iff deterministic key generation is supported

        NOTE: FIPS 203 abbreviates key generation as "KeyGen", so that's what we
        will go with here.
        """

        self.derandomized_encaps = derandomized_encaps
        """True iff deterministic encapsulation is supported

        NOTE: FIPS 203 abbreviates encapsulation as "Encaps", so that's what we
        will go with here.
        """

        self.params = params
        """A map from parameter key (e.g. ml_kem_512) to a parameter set"""

        self.impls = impls
        """A map from implementation key (e.g. mlkem-native_ml-kem-1024_x86_64)
        to an implementation"""

    @staticmethod
    def from_oqsbuildfile(key: str, kem_meta: dict):
        version = kem_meta["version"]
        header = kem_meta.get(
            "header", f"{CryptoPrimitive.KEM.get_subdirectory_name()}_{key}.h"
        )
        derand_keygen = kem_meta.get("derandomized_keypair", False)
        derand_encaps = kem_meta.get("derandomized_encaps", False)
        params = {
            param_key: ParameterSet.from_oqsbuildfile(param_key, param_meta)
            for param_key, param_meta in kem_meta["params"].items()
        }
        impls = {
            impl_key: Implementation.from_oqsbuildfile(impl_key, impl_meta)
            for impl_key, impl_meta in kem_meta["impls"].items()
        }
        return KemFamily(
            key, version, header, derand_keygen, derand_encaps, params, impls
        )

    def generate_header(self, kem_dir: str, autoformat: bool = True) -> str:
        """Generate the family-level header file, such as
        LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem.h.

        Return the path to the generated header file.
        """
        header_path = os.path.join(kem_dir, self.header)

        header = load_jinja_template(
            os.path.join(LIBOQS_JINJA_TEMPLATES_DIR, KEM_HEADER_TEMPLATE)
        ).render(
            {
                "kem_key": self.key,
                "kem_meta": self,
                "generated_by": f"scripts/oqsbuilder.py:{__class__.__name__}.{currentframe_funcname()}",
            }
        )
        with open(header_path, "w") as f:
            f.write(header)
        if autoformat:
            format_with_astyle(header_path)
        return header_path

    def generate_cmake(self, kem_dir: str, autoformat: bool = True) -> str:
        """Generate the family-level CMakeLists.txt file for the input KEM scheme

        Each family-level list file (e.g. src/kem/ml_kem/CMakeLists.txt) exports a
        cmake variable (e.g. ML_KEM_OBJS) that contains the compiled objects from
        that family.

        :param autoformat: format the generated list file with gersemi
        :return: path to the family-level cmake list file
        """
        template_path = os.path.join(LIBOQS_JINJA_TEMPLATES_DIR, KEM_CMAKE_TEMPLATE)
        cmake_path = os.path.join(kem_dir, "CMakeLists.txt")
        content = load_jinja_template(template_path).render(
            {
                "kem_key": self.key,
                "kem_meta": self,
                "generated_by": f"scripts/oqsbuilder.py:{__class__.__name__}.{currentframe_funcname()}",
                "CmakeScope": CmakeScope,
            }
        )
        # print(content)
        # raise NotImplementedError(f"{cmake_path} is not fully rendered")
        with open(cmake_path, "w") as f:
            f.write(content)
        if autoformat:
            # Check out gersemi at https://github.com/BlankSpruce/gersemi/
            # pip install gersemi==0.23.1
            subprocess.run(["gersemi", "-i", cmake_path], check=True)
        return cmake_path

    def generate_sources(self, kem_dir: str, autoformat: bool = True) -> list[str]:
        """Generate the family-level source file(s), such as
        LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem_<512|768|1024>.c
        """
        source_paths = []
        for param_key, param_meta in self.params.items():
            template = load_jinja_template(
                os.path.join(LIBOQS_JINJA_TEMPLATES_DIR, KEM_SRC_TEMPLATE)
            )
            default_impl = self.impls[param_meta.default_impl_key]
            addtl_impls = [
                impl
                for impl_key, impl in self.impls.items()
                if impl_key != param_meta.default_impl_key
                and impl.param_key == param_key
            ]
            addtl_impls_keypair_derand = [
                impl for impl in addtl_impls if impl.api_names.keypair_derand
            ]
            addtl_impls_encaps_derand = [
                impl for impl in addtl_impls if impl.api_names.encaps_derand
            ]
            content = template.render(
                {
                    "kem_key": self.key,
                    "param_key": param_key,
                    "nist_level": param_meta.nist_level,
                    "ind_cca": param_meta.ind_cca,
                    "version": self.version,
                    "derandomized_keypair": self.derandomized_keypair,
                    "derandomized_encaps": self.derandomized_encaps,
                    "default_impl": default_impl,
                    "addtl_impls": addtl_impls,
                    "addtl_impls_keypair_derand": addtl_impls_keypair_derand,
                    "addtl_impls_encaps_derand": addtl_impls_encaps_derand,
                    "generated_by": f"scripts/oqsbuilder.py:{__class__.__name__}.{currentframe_funcname()}",
                }
            )
            src_path = os.path.join(kem_dir, f"kem_{param_key}.c")
            with open(src_path, "w") as f:
                f.write(content)
            if autoformat:
                format_with_astyle(src_path)
            source_paths.append(src_path)
        return source_paths


class Upstream:
    """A git repository containing some source files"""

    def __init__(
        self,
        key: str,
        url: str,
        branch_or_tag: str | None = "main",
        commit: str | None = None,
        patches: list[str] | None = None,
    ):
        if (not branch_or_tag) and (not commit):
            raise ValueError("branch_or_tag and commit cannot both be None")
        self.key = key
        """unique identifier of the upstream repository, such as \"mlkem-native\""""
        self.url = url
        """required, URL of the remote repository"""

        self.branch_or_tag = branch_or_tag
        """optional, check out the specified branch or tag after clone"""

        self.commit = commit
        """optional, check out the specified commit after clone.

        Commit will overwrite branch/tag. At least one of these two should be supplied."""

        self.patches = patches or []
        """absoluate paths to patch files to be applied after cloning

        in oqsbuildfile, patch paths are relative to some patch directory; at load_oqsbuildfile
        the patch directory will be prefixed onto the paths"""

        self.dir: str | None = None
        """path to the cloned repository, or None if not yet cloned"""

        self._patched = len(self.patches) == 0
        """True iff all patches have been applied"""

    @staticmethod
    def from_oqsbuildfile(key: str, data: dict):
        return Upstream(
            key,
            data["git_url"],
            data.get("git_branch", None),
            data.get("git_commit", None),
            data.get("patches", None),
        )

    def clone(
        self, parentdir: str | os.PathLike, dstdirname: str, dryrun: bool = False
    ):
        """Clone a remote Git repository into a local destination directory.

        :param parentdir: Path to the parent directory where the repository will be cloned.
        :param dstdirname: Name of the destination directory to create within `parentdir`.
        :param url: URL of the remote Git repository to clone.
        :param commit: Optional specific commit hash to check out after cloning.
            If provided, this takes precedence over `branch_or_tag`.
        :param branch_or_tag: Optional branch or tag name to clone.
            Ignored if `commit` is specified.
        :param dryrun: if set to true, print the commands that will be executed, but
            do not execute them.
        :return: The full path to the cloned repository directory.
        """
        if not os.path.isdir(parentdir):
            raise FileNotFoundError(f"{parentdir} is not a valid directory")
        dstdir = os.path.join(parentdir, dstdirname)
        if os.path.isdir(dstdir):
            raise FileExistsError(f"{dstdir} already exists")
        if dryrun:
            print(f"mkdir -p {dstdir}")
        else:
            # NOTE: dstdir could contain forward slashes; create nested directories w/ makedirs
            os.makedirs(dstdir)
        gitdir = os.path.join(dstdir, ".git")
        if self.commit:
            # git clone <url> <dst> --depth 1
            # git fetch --git-dir ... --work-tree ... fetch origin <sha1> --depth 1
            # git reset --git-dir ... --work-tree ... reset --hard <sha1>
            commands = [
                ["git", "clone", self.url, dstdir, "--depth", "1"],
                ["git", "--git-dir", gitdir, "--work-tree", dstdir]
                + ["fetch", "origin", self.commit, "--depth", "1"],
                ["git", "--git-dir", gitdir, "--work-tree", dstdir]
                + ["reset", "--hard", self.commit],
            ]
        elif self.branch_or_tag:
            commands = [
                [
                    "git",
                    "clone",
                    self.url,
                    dstdir,
                    "--branch",
                    self.branch_or_tag,
                    "--depth",
                    "1",
                ]
            ]
        else:
            commands = [["git", "clone", self.url, dstdir, "--depth", "1"]]
        for cmd in commands:
            if dryrun:
                print(" ".join(cmd))
            else:
                subprocess.run(cmd, check=True)
        self.dir = dstdir

    def patch(
        self,
        commit_after_apply: bool = True,
        dryrun: bool = False,
    ):
        """Apply a patch to the specified git repository

        Note that the --unsafe-paths in the "git apply" command is necessary

        :param commit_after_apply: commit the changes after applying the patch
        :param dryrun: If True, print the commands instead of executing them
        """
        if not self.dir:
            raise FileNotFoundError("upstream not cloned")
        if len(self.patches) == 0:
            return
        for patch in self.patches:
            if not os.path.isfile(patch):
                raise FileNotFoundError(f"{patch} is not a valid patch file")

        gitdir = os.path.join(self.dir, ".git")
        worktree = directory = self.dir
        git_apply_cmd = (
            ["git", "--git-dir", gitdir, "--work-tree", worktree]
            + ["apply", "--unsafe-paths", "--verbose", "--whitespace", "fix"]
            + ["--directory", directory]
        )
        git_apply_cmd += self.patches
        commands = [git_apply_cmd]
        if commit_after_apply:
            patch_names: list[str] = []
            for patch in self.patches:
                _, patch_filename = os.path.split(patch)
                patch_name, _ = os.path.splitext(patch_filename)
                patch_names.append(patch_name)
            commit_msg = f"Applied {', '.join(patch_names)}"

            commands.append(
                ["git", "--git-dir", gitdir, "--work-tree", worktree, "add", "-A"]
            )
            commands.append(
                ["git", "--git-dir", gitdir, "--work-tree", worktree]
                + ["commit", "-m", commit_msg]
            )
        for cmd in commands:
            if dryrun:
                print(" ".join(cmd))
            else:
                subprocess.run(cmd, check=True)
                self._patched = True


class OQSBuilder:
    def __init__(
        self,
        upstreams: dict[str, Upstream],
        kems: dict[str, KemFamily],
        patch_dir: str | os.PathLike,
    ):
        # TODO: validate input?
        self.upstreams = upstreams
        self.kems = kems
        self.patch_dir = patch_dir

    @staticmethod
    def to_literal_copies(
        buildfile_copies: str | LiteralCopiesMap,
        reusable_local_copies: dict[str, LiteralCopiesMap],
    ) -> LiteralCopiesMap:
        """Given the value under the "copies" field in some implementation, return
        the true copy mapping (dst -> src)

        :param buildfile_copies: the value of the "copies" field in some implementation,
        could be one of "literal mapping", "reusable local key", or "remote URL"
        """
        if isinstance(buildfile_copies, dict):
            return buildfile_copies
        # TODO: for now all liboqs integration meta file ends with "META.yml`,
        #   in the future we need more robust logic?
        if buildfile_copies.endswith("META.yml"):
            # FIX: not implemented
            raise NotImplementedError(
                "Fetching copy map from META.yml is not supported yet"
            )
        if buildfile_copies not in reusable_local_copies:
            raise KeyError(f"{buildfile_copies} is not a valid reusable local copy key")
        return reusable_local_copies[buildfile_copies]

    @staticmethod
    def load_oqsbuildfile(path: str | os.PathLike):
        with open(path, mode="r", encoding="utf-8") as f:
            oqsbuild = yaml.safe_load(f)

        # FIX: refactor these pre-processing actions into a "preprocess_oqsbuildfile"
        if True:
            for _, family_meta in oqsbuild["kems"]["families"].items():
                for _, impl_meta in family_meta["impls"].items():
                    impl_meta["copies"] = OQSBuilder.to_literal_copies(
                        impl_meta["copies"], oqsbuild["copies"]
                    )
                for param_key, param_meta in family_meta["params"].items():
                    default_impl_key = param_meta["default_impl"]
                    if default_impl_key not in family_meta["impls"]:
                        raise KeyError(
                            f"{param_key}'s default impl {default_impl_key} not found"
                        )
                    default_impl_meta = family_meta["impls"][default_impl_key]
                    default_impl_meta["enable_by"] = param_meta["enable_by"]
            for _, upstream in oqsbuild["upstreams"].items():
                patch_paths = [
                    os.path.join(LIBOQS_PATCH_DIR, patch_path)
                    for patch_path in upstream.get("patches", [])
                ]
                upstream["patches"] = patch_paths

        upstreams = {
            key: Upstream.from_oqsbuildfile(key, meta)
            for key, meta in oqsbuild["upstreams"].items()
        }
        kems = {
            key: KemFamily.from_oqsbuildfile(key, meta)
            for key, meta in oqsbuild["kems"]["families"].items()
        }

        return OQSBuilder(upstreams, kems, LIBOQS_PATCH_DIR)

    def fetch_upstreams(self, upstream_parent_dir: str | os.PathLike):
        for upstream_key, upstream in self.upstreams.items():
            upstream.clone(upstream_parent_dir, upstream_key)
            upstream.patch()

    def build_kems(self):
        """For each KEM family, create the KEM subdirectory, copy over the source files,
        then generate the family-level header/source/cmake files
        """
        kems_dir = os.path.join(
            LIBOQS_SRC_DIR, CryptoPrimitive.KEM.get_subdirectory_name()
        )
        for kem_key, kem in self.kems.items():
            kem_dir = os.path.join(kems_dir, kem_key)
            print(f"Integrating {kem_key} into {kem_dir}")
            for impl_key, impl in kem.impls.items():
                impl_dir = os.path.join(kem_dir, impl_key)
                upstream = self.upstreams[impl.upstream_key]
                if (not upstream.dir) or (not upstream._patched):
                    raise FileNotFoundError(
                        f"Upstream {impl.upstream_key} is not cloned"
                    )
                copy_copies(impl.copies, upstream.dir, impl_dir)
            kem.generate_cmake(kem_dir, autoformat=OQSBUILDER_AUTOFORMAT_CMAKE)
            kem.generate_header(kem_dir)
            kem.generate_sources(kem_dir)


def copy_copies(copies: dict[str, str], upstream_dir: str, impl_dir: str):
    """Copy the specified file from upstream_dir into impl_dir.

    :param copies: mapping from destination paths (relative to implementation
    directory) to source paths (relative to upstream directory)
    :param upstream_dir: path to the upstream directory
    :param impl_dir: path to the implementation directory
    """
    for dst, src in copies.items():
        src = os.path.join(upstream_dir, src)
        dst = os.path.join(impl_dir, dst)
        dst_parent_dir = os.path.split(dst)[0]
        if not os.path.isdir(dst_parent_dir):
            print(f"mkdir -p {dst_parent_dir}")
            os.makedirs(dst_parent_dir)
        shutil.copyfile(src, dst)
    print(f"Copied {len(copies)} files into {impl_dir}")


def format_with_astyle(path: str):
    """Call astyle to format file at the input path"""
    options_path = os.path.join(LIBOQS_DIR, ".astylerc")
    subprocess.run(
        ["astyle", f"--options={options_path}", '--suffix=""', path], check=True
    )


def currentframe_funcname() -> str:
    """Retrieves the name of the caller's function using stack inspection.

    Iterates through the current call stack to find the frame object of the
    immediate caller. It accesses the code object associated with that frame
    to extract the function name.

    This function relies on CPython implementation details (stack frames).
    It may not behave identically in other Python implementations like PyPy.

    :return: The name of the function that invoked this utility.
    """
    stack = inspect.stack()
    if len(stack) < 2:
        raise RuntimeError("current frame is missing a caller")
    return stack[1].frame.f_code.co_name


def load_jinja_template(path: str) -> jinja2.Template:
    """Read from the input path, then instantiate a template object"""
    with open(path, "r", encoding="utf-8") as f:
        template_str = f.read()
    return jinja2.Template(template_str)


def print_version():
    """Print version and system info"""
    print(f"OQS Builder v{__version__}")
    print("PYTHONPATH:")
    for path in sys.path:
        print(f"-   {path}")


def copy_from_upstream(_: argparse.Namespace):
    """Copy implementations from upstream

    on MacOS, TemporaryDirectory() is created under /var, which is
    a symlink to private/var, but git considers tracking symlinks to be
    unsafe and will refuse to run. Hence we need to use a base directory
    that has not symlink component, such as LIBOQS_DIR. Fortunately,
    all of TemporaryDirectory's automatic cleanup still works.

    :param oqsbuildfile: path to copy_from_upstream.yml
    :param upstream_parent_dir: upstream repositories will be cloned into
        a temporary subdirectory under this directory
    :param templates_dir: path to a directory containing the Jinja2 templates
        used to generate CMakeLists.txt files, source files, and header files
    :param headless: True if running in a non-interactive environment
    """
    oqsbuildfile = OQSBUILDFILE_PATH
    templates_dir = LIBOQS_JINJA_TEMPLATES_DIR
    upstream_parent_dir = LIBOQS_DIR
    assert os.path.isfile(oqsbuildfile), f"{oqsbuildfile} is not a valid file"
    assert os.path.isdir(templates_dir), f"{templates_dir} is not a valid directory"
    assert os.path.isdir(
        upstream_parent_dir
    ), f"{upstream_parent_dir} is not a valid directory"

    oqsbuilder = OQSBuilder.load_oqsbuildfile(oqsbuildfile)
    with TemporaryDirectory(dir=upstream_parent_dir) as tempdir:
        print(f"upstreams will be cloned into {tempdir}")
        oqsbuilder.fetch_upstreams(tempdir)

        oqsbuilder.build_kems()


def print_diagnostics(_: argparse.Namespace):
    keys = [
        "LIBOQS_DIR",
        "LIBOQS_PATCH_DIR",
        "LIBOQS_SRC_DIR",
        "LIBOQS_JINJA_TEMPLATES_DIR",
        "OQSBUILDER_AUTOFORMAT_CMAKE",
        "OQSBUILDFILE_PATH",
    ]
    for key in sorted(keys):
        val = globals()[key]
        print(f"{key:32}:{val}")


def main():
    parser = argparse.ArgumentParser(
        prog="oqsbuilder.py",
        description="OQS Builder",
    )

    subparsers = parser.add_subparsers(required=True, help="subcommands")
    subparser_diagnose = subparsers.add_parser(
        "diagnose", help="print diagnostic information"
    )
    subparser_diagnose.set_defaults(func=print_diagnostics)
    subparser_copy = subparsers.add_parser(
        "copy",
        help="build liboqs sources",
    )
    subparser_copy.set_defaults(func=copy_from_upstream)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    # print_version()
    # copy_from_upstream(OQSBUILDFILE_PATH, LIBOQS_JINJA_TEMPLATES_DIR)
    main()
