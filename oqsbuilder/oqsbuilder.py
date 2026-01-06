import enum
import os
import shutil
import subprocess

import yaml

from oqsbuilder import LIBOQS_DIR
from oqsbuilder.utils import currentframe_funcname, load_jinja_template

SRC_FILE_EXTS = (".c", ".s", ".S", ".cpp", ".cu")

CPU_RUNTIME_FEATURES = ("asimd", "avx2", "bmi2", "popcnt")
CPU_RUNTIME_FEATURES_MAP = {"asimd": "arm_neon"}

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
    def __init__(
        self,
        key: str,
        name: str,
        pubkeylen: int,
        privkeylen: int,
        ctlen: int,
        sslen: int,
        enable_by: str,
        default_impl: str,
        nist_level: NistLevel,
        ind_cca: bool,
    ):
        self.key = key
        """Unique identifier of parameter set, such as ml_kem_512"""

        self.name = name
        """Human-friendly name of parameter set, such as \"ML-KEM-512\""""

        self.pubkeylen = pubkeylen
        """Length of encapsulation key in bytes"""

        self.privkeylen = privkeylen
        """Length of decapsulation key in bytes"""

        self.ctlen = ctlen
        """Length of ciphertext in bytes"""

        self.sslen = sslen
        """Length of shared secret in bytes"""

        self.enable_by = enable_by
        """A C pre-processing macro that enables this parameter set.

        If a parameter set if disabled, then all implementations of this parameter
        set are disabled"""

        self.default_impl = default_impl
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


class Implementation:
    pass


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
        header: str | None = None,
        derandomized_keygen: bool = False,
        derandomized_encaps: bool = False,
        params: dict[str, ParameterSet] | None = None,
        impls: dict[str, ParameterSet] | None = None,
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

        self.derandomized_keygen = derandomized_keygen
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


class Upstream:
    """A git repository containing some source files"""

    def __init__(
        self,
        url: str,
        branch_or_tag: str | None = "main",
        commit: str | None = None,
        patches: list[str] | None = None,
    ):
        """
        :param url: required, URL of the remote repository
        :param branch_or_tag: optional, check out the specified branch or tag after clone
        :param commit: optional, check out the specified commit after clone. This
            will overwrite branch_or_tag. At least one of these two should be supplied.
        :param patches: path to a single patch file, or a list of many patch files;
            if a list of patches is passed in, then all of them will be applied in
            a single "git apply" command. Empty patch list will be ignored.
        """
        if not url:
            raise ValueError("url cannot be None")
        if (not branch_or_tag) and (not commit):
            raise ValueError("branch_or_tag and commit cannot both be None")
        self.url = url
        self.branch_or_tag = branch_or_tag
        self.commit = commit
        self.patches = patches or []

    @staticmethod
    def from_dict(data: dict):
        return Upstream(
            data["git_url"],
            data.get("git_branch", None),
            data.get("git_commit", None),
            data.get("patches", None),
        )

    def clone(self, parentdir: str, dstdirname: str, dryrun: bool = False) -> str:
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
        return dstdir


def load_runtime_cpu_features(features: list[str]) -> list[str]:
    """Read the list of runtime CPU features, check the validity of each feature
    name, and map to the appropriate names
    """
    for feat in features:
        if feat not in CPU_RUNTIME_FEATURES:
            raise ValueError(f"{feat} is not valid runtime CPU feature")
    return [CPU_RUNTIME_FEATURES_MAP.get(feat, feat) for feat in features]


def load_oqsbuildfile(path: str):
    """Load oqsbuildfile from the specified path

    For each implementation, if the `copies` field is mapped to a `copies` key,
    then the `copies` field will be instantiated with the actual dst:src mapping
    under the top-level `copies` section.
    """
    with open(path, mode="r", encoding="utf-8") as f:
        oqsbuild = yaml.safe_load(f)

    # Expand keys and fill in defaults
    for primitive in [
        CryptoPrimitive.KEM,
        # CryptoPrimitive.SIG,
        # CryptoPrimitive.STFL_SIG,
    ]:
        for family_key, family in oqsbuild[primitive.get_oqsbuildfile_key()][
            "families"
        ].items():
            family["header"] = family.get(
                "header", f"{primitive.get_subdirectory_name()}_{family_key}.h"
            )
            family["derandomized_keypair"] = family.get("derandomized_keypair", False)
            for param_key, param_meta in family["params"].items():
                param_meta["api_src"] = param_meta.get(
                    "api_src", f"{primitive.get_subdirectory_name()}_{param_key}.c"
                )
                _, default_impl_meta = get_default_impl(family, param_key)
                default_impl_meta["enable_by"] = param_meta["enable_by"]
            for _, impl_meta in family["impls"].items():
                impl_copies = impl_meta["copies"]
                if isinstance(impl_copies, str):
                    impl_meta["copies"] = oqsbuild["copies"][impl_copies]
                # NOTE: this field does not exist in oqsbuildfile
                impl_meta["cmake_src_paths"] = [
                    os.path.join("${IMPL_KEY}", path)
                    for path in impl_meta["copies"]
                    if os.path.splitext(path)[1] in SRC_FILE_EXTS
                ]
                impl_arch_key = impl_meta["arch"]
                impl_meta["arch"] = oqsbuild["architectures"][impl_arch_key]
                impl_meta["runtime_cpu_features"] = load_runtime_cpu_features(
                    impl_meta.get("runtime_cpu_features", [])
                )

    return oqsbuild


def get_copies(
    oqsbuild: dict, primitive: CryptoPrimitive, family_key: str, impl_key: str
) -> dict[str, str]:
    """Return the copy dictionary of the specified implementation. A copy
    dictionary maps destination path to source path. Destination path is relative
    to the implementation sub-directory. Source path is relative to the upstream
    repostiroy's root directory.

    :param oqsbuild: the data in oqsbuildfile
    :param primitive: indicates whether to look under kems, sigs, or stfl_sigs
        section under oqsbuildfile
    :param family_key: the family key, such as "ml_kem"
    :param impl_key: the implementation key, such as "mlkem-native_ml-kem-512_ref"
    :return: a map from destination paths to source paths
    """
    family = oqsbuild[primitive.get_oqsbuildfile_key()]["families"][family_key]
    impl = family["impls"][impl_key]
    impl_copies: str | dict[str, str] = impl["copies"]
    if isinstance(impl_copies, str):
        return oqsbuild["copies"][impl_copies]
    elif isinstance(impl_copies, dict):
        return impl_copies
    raise TypeError(
        f"Invalid type for {family_key}.{impl_key}.copies {type(impl_copies)}"
    )


def get_git() -> str | None:
    """Check that git exists under current environment

    :return: git version if git is found, None if git is not found
    """
    ret = subprocess.run(["git", "--version"], encoding="utf-8", capture_output=True)
    if ret.returncode == 0:
        # Example output "git version 2.51.1\n"
        return ret.stdout.strip().split()[-1]
    return None


def git_apply(
    dstdir: str,
    patches: str | list[str],
    gitdir: str | None = None,
    worktree: str | None = None,
    directory: str | None = None,
    commit_after_apply: bool = True,
    commit_msg: str | None = None,
    dryrun: bool = False,
):
    """Apply a patch to the specified git repository

    Note that the --unsafe-paths in the "git apply" command is necessary

    :param dstdir: path to the git repository on which the patch will be applied
    :param patches: path to a single patch file, or a list of many patch files;
        if a list of patches is passed in, then all of them will be applied in
        a single "git apply" command. Empty patch list will be ignored.
    :param gitdir: path to the .git directory, defaults to {dstdir}/.git
    :param worktree: path to the worktree, defaults to {dstdir}
    :param directory: prepend to filenames in the patch file, see
        "git apply --directory=<root>", defaults to {dstdir}
    :param commit_after_apply: if True, commit the changes after applying the patch
    :param commit_msg: specify a commit message if commit_after_apply, defaults
        to "applied {patch1}, {patch2}, ..."
    :param dryrun: If True, print the commands instead of executing them
    """
    if not os.path.isdir(dstdir):
        raise FileNotFoundError(f"{dstdir} is not a valid directory")
    gitdir = os.path.join(dstdir, ".git") if not gitdir else gitdir
    if not os.path.isdir(gitdir):
        raise FileNotFoundError(f"{gitdir} is not a valid .git directory")
    worktree = dstdir if not worktree else worktree
    if not os.path.isdir(worktree):
        raise FileNotFoundError(f"{worktree} is not a valid git work tree")
    directory = dstdir if not directory else directory
    if not os.path.isdir(directory):
        raise FileNotFoundError(f"{directory} is not a valid directory")
    patches = [patches] if isinstance(patches, str) else patches
    if len(patches) == 0:
        return
    for patch in patches:
        if not os.path.isfile(patch):
            raise FileNotFoundError(f"{patch} is not a valid patch file")

    if not commit_msg:
        patch_names: list[str] = []
        for patch in patches:
            _, patch_filename = os.path.split(patch)
            patch_name, _ = os.path.splitext(patch_filename)
            patch_names.append(patch_name)
        commit_msg = f"Applied {', '.join(patch_names)}"

    git_apply_cmd = (
        ["git", "--git-dir", gitdir, "--work-tree", worktree]
        + ["apply", "--unsafe-paths", "--verbose", "--whitespace", "fix"]
        + ["--directory", directory]
    )
    git_apply_cmd += patches
    commands = [git_apply_cmd]
    if commit_after_apply:
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


def fetch_upstreams(
    oqsbuild: dict, upstream_parent_dir: str, patch_dir: str
) -> dict[str, str]:
    """Clone upstream repositories into the specified parent directory and apply
    patches. Return a mapping from upstream key to path to the upstream repository
    """
    upstream_dirs = {}
    for name, upstream in oqsbuild["upstreams"].items():
        # TODO: move Upstream.from_dict further up, into load_oqsbuildfile
        upstream = Upstream.from_dict(upstream)
        upstream_dir = upstream.clone(upstream_parent_dir, name)
        patches: list[str] = [
            os.path.join(patch_dir, patch) for patch in upstream.patches
        ]
        git_apply(upstream_dir, patches)
        upstream_dirs[name] = upstream_dir
    print(f"SUCCESS: fetched {len(upstream_dirs)} upstream repositories")
    return upstream_dirs


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


def get_default_impl(family: dict, param_key: str) -> tuple[str, dict]:
    """Get the implementation key and the implementation metadata for the
    specified parameter set under the given family

    :return: a tuple of (impl_key, impl_meta)
    """
    impl_key = family["params"][param_key]["default_impl"]
    impl = family["impls"][impl_key]
    impl_param_key = impl["param"]
    if impl_param_key != param_key:
        raise ValueError(
            f"{param_key}'s default impl {impl_key} specified param set {impl_param_key}"
        )
    return impl_key, impl


def get_impls(
    family: dict, param_key: str, exclude_default: bool = False
) -> list[tuple[str, dict]]:
    """Return a list of (impl_key, impl_metadata) for the specified parameter set"""
    impls = []
    default_impl_key, _ = get_default_impl(family, param_key)
    for impl_key, impl in family["impls"].items():
        exclude = exclude_default and (impl_key == default_impl_key)
        if impl["param"] == param_key and (not exclude):
            impls.append((impl_key, impl))
    return impls


def generate_kem_cmake(
    kem_dir: str,
    kem_key: str,
    kem_meta: dict,
    templates_dir: str,
    autoformat: bool = True,
) -> str:
    """Generate the family-level CMakeLists.txt file for the input KEM scheme

    Each family-level list file (e.g. src/kem/ml_kem/CMakeLists.txt) exports a
    cmake variable (e.g. ML_KEM_OBJS) that contains the compiled objects from
    that family.

    :param kem_dir: path to the family-level subdirectory, such as
        LIBOQS_DIR/src/kem/ml_kem
    :param kem_key: the family key of the KEM scheme
    :param kem_meta: metadata of the KEM family
    :param templates_dir: path to the directory containing jinja templates
    :param autoformat: format the generated list file with gersemi
    :return: path to the family-level cmake list file
    """
    template_path = os.path.join(templates_dir, KEM_CMAKE_TEMPLATE)
    cmake_path = os.path.join(kem_dir, "CMakeLists.txt")
    content = load_jinja_template(template_path).render(
        {
            "kem_key": kem_key,
            "kem_meta": kem_meta,
            "generated_by": f"{__name__}.{currentframe_funcname()}",
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


def format_with_astyle(path: str):
    """Call astyle to format file at the input path"""
    options_path = os.path.join(LIBOQS_DIR, ".astylerc")
    subprocess.run(
        ["astyle", f"--options={options_path}", '--suffix=""', path], check=True
    )


def generate_kem_header(
    kem_dir: str,
    kem_key: str,
    kem_meta: dict,
    templates_dir: str,
    autoformat: bool = True,
) -> str:
    """Generate the family-level header file, such as
    LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem.h.

    Return the path to the generated header file.
    """
    header_path = os.path.join(kem_dir, kem_meta["header"])

    header = load_jinja_template(
        os.path.join(templates_dir, KEM_HEADER_TEMPLATE)
    ).render(
        {
            "kem_key": kem_key,
            "kem_meta": kem_meta,
            "generated_by": f"{__name__}.{currentframe_funcname()}",
        }
    )
    with open(header_path, "w") as f:
        f.write(header)
    if autoformat:
        format_with_astyle(header_path)
    return header_path


def generate_kem_source(
    kem_dir: str,
    kem_key: str,
    kem_meta: dict,
    param_key: str,
    param_meta: dict,
    templates_dir: str,
    autoformat: bool = True,
) -> str:
    """Render source file implementing OQS' API or the given KEM parameter set
    (such as kem_ml_kem_768.c), write the source code to the appropriate location,
    then return the path to that source file

    :param templates_dir: path to the directory containing the Jinja templates
    :return: path to the rendered source file, such as src/kem/ml_kem/kem_ml_kem_768.c
    """
    template = load_jinja_template(os.path.join(templates_dir, KEM_SRC_TEMPLATE))
    _, default_impl = get_default_impl(kem_meta, param_key)
    addtl_impls = [impl for _, impl in get_impls(kem_meta, param_key, True)]
    addtl_impls_keypair_derand = [
        impl for impl in addtl_impls if impl.get("keypair_derand", None)
    ]
    addtl_impls_encaps_derand = [
        impl for impl in addtl_impls if impl.get("enc_derand", None)
    ]
    content = template.render(
        {
            "kem_key": kem_key,
            "param_key": param_key,
            "nist_level": param_meta["nist_level"],
            "ind_cca": param_meta["ind_cca"],
            "version": kem_meta["version"],
            "derandomized_keypair": kem_meta["derandomized_keypair"],
            "derandomized_encaps": kem_meta["derandomized_encaps"],
            "default_impl": default_impl,
            "addtl_impls": addtl_impls,
            "addtl_impls_keypair_derand": addtl_impls_keypair_derand,
            "addtl_impls_encaps_derand": addtl_impls_encaps_derand,
            "generated_by": f"{__name__}.{currentframe_funcname()}",
        }
    )
    src_path = os.path.join(kem_dir, f"kem_{param_key}.c")
    with open(src_path, "w") as f:
        f.write(content)
    if autoformat:
        format_with_astyle(src_path)
    return src_path


def generate_kem_sources(
    kem_dir: str,
    kem_key: str,
    kem_meta: dict,
    templates_dir: str,
    autoformat: bool = True,
) -> list[str]:
    """Generate the family-level source file(s), such as
    LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem_<512|768|1024>.c
    """
    source_paths = []
    for param_key, param_meta in kem_meta["params"].items():
        source_path = generate_kem_source(
            kem_dir, kem_key, kem_meta, param_key, param_meta, templates_dir, autoformat
        )
        source_paths.append(source_path)
    return source_paths
