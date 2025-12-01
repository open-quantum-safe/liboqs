import enum
import os
import shutil
import subprocess

import yaml

from oqsbuilder import LIBOQS_DIR
from oqsbuilder.templates import (
    SPDX_LICENSE_IDENTIFIER,
    NIST_LEVELS,
    OQS_KEM_NEW_IMPL,
)
from oqsbuilder.utils import currentframe_funcname

SRC_FILE_EXTS = (".c", ".s", ".S", ".cpp", ".cu")
SCOPE_OPTIONS = ("public", "private", "interface")


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
            for _, impl_meta in family["impls"].items():
                impl_copies = impl_meta["copies"]
                if isinstance(impl_copies, str):
                    impl_meta["copies"] = oqsbuild["copies"][impl_copies]
                impl_arch_key = impl_meta["arch"]
                impl_meta["arch"] = oqsbuild["architectures"][impl_arch_key]

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


def clone_remote_repo(
    parentdir: str,
    dstdirname: str,
    url: str,
    commit: str | None = None,
    branch_or_tag: str | None = None,
    dryrun: bool = False,
) -> str:
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
    if commit:
        # git clone <url> <dst> --depth 1
        # git fetch --git-dir ... --work-tree ... fetch origin <sha1> --depth 1
        # git reset --git-dir ... --work-tree ... reset --hard <sha1>
        commands = [
            ["git", "clone", url, dstdir, "--depth", "1"],
            ["git", "--git-dir", gitdir, "--work-tree", dstdir]
            + ["fetch", "origin", commit, "--depth", "1"],
            ["git", "--git-dir", gitdir, "--work-tree", dstdir]
            + ["reset", "--hard", commit],
        ]
    elif branch_or_tag:
        commands = [
            ["git", "clone", url, dstdir, "--branch", branch_or_tag, "--depth", "1"]
        ]
    else:
        commands = [["git", "clone", url, dstdir, "--depth", "1"]]
    for cmd in commands:
        if dryrun:
            print(" ".join(cmd))
        else:
            subprocess.run(cmd, check=True)
    return dstdir


def fetch_upstreams(
    oqsbuild: dict, upstream_parent_dir: str, patch_dir: str
) -> dict[str, str]:
    """Clone upstream repositories into the specified parent directory and apply
    patches. Return a mapping from upstream key to path to the upstream repository
    """
    upstream_dirs = {}
    for name, upstream in oqsbuild["upstreams"].items():
        upstream_dir = clone_remote_repo(
            upstream_parent_dir,
            name,
            upstream["git_url"],
            commit=upstream.get("git_commit", None),
            branch_or_tag=upstream.get("git_branch", None),
        )
        patches: list[str] = [
            os.path.join(patch_dir, patch) for patch in upstream.get("patches", [])
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


def get_impl_compile_opts(impl_meta: dict, scope: str) -> list[str] | None:
    """Return the list of public compiler options or None if there is none"""
    assert scope in SCOPE_OPTIONS, f"scope must be in {SCOPE_OPTIONS}"
    compile_opts = impl_meta.get("compile_opts", None)
    if not compile_opts:
        return None
    return compile_opts.get(scope, None)


def get_impl_include_dirs(impl_meta: dict, scope: str) -> list[str] | None:
    """Return the list of include directories or None"""
    assert scope in SCOPE_OPTIONS, f"scope must be in {SCOPE_OPTIONS}"
    include_dirs = impl_meta.get("includes", None)
    if not include_dirs:
        return None
    return include_dirs.get(scope, None)


# TODO: get_impl_include_dirs, get_impl_link_libs, and get_impl_compile_opts
#   are highly similar. Consider refactoring them into a single function
def get_impl_link_libs(impl_meta: dict, scope: str) -> list[str] | None:
    """Return the list of include directories or None"""
    assert scope in SCOPE_OPTIONS, f"scope must be in {SCOPE_OPTIONS}"
    include_dirs = impl_meta.get("link_libs", None)
    if not include_dirs:
        return None
    return include_dirs.get(scope, None)


def generate_family_cmake_targets(
    family_key: str,
    family_meta: dict,
    local_obj: str,
    overwrite_default_impl_enable_by: bool = True,
) -> list[str]:
    """Generate a list of family-level CMake fragments where each fragment builds
    an object library target. Some targets contain individual implementations,
    such as mlkem-native_ml-kem-512_ref. Other targets contain OQS APIs, such
    as OQS_KEM_ml_kem_512_new (specified in kem_ml_kem_512.c)

    :param family_key: key of a family, such as ml_kem or ml_dsa
    :param family_meta: this family's metadata
    :param local_obj: the non-exported cmake variable that aggregates objects
        for this family, such as `_ML_KEM_OBJS` in src/kem/ml_kem/CMakeLists.txt
    :param overwrite_default_impl_enable_by: for each parameter set, overwrite
        the `enable_by` flag of the default implementation with the `enable_by`
        flag of the parameter set
    """
    common_targets = []
    for param_key, param_meta in family_meta["params"].items():
        print(f"Generating common targets for {family_key}.{param_key}")
        param_enable_by = param_meta["enable_by"]
        param_api_src = param_meta["api_src"]
        target = f"""\
if({param_enable_by})
    add_library({param_key} OBJECT {param_api_src})
    set({local_obj} ${{{local_obj}}} $<TARGET_OBJECTS:{param_key}>)
endif()"""
        common_targets.append(target)

    impl_targets = []
    for impl_key, impl_meta in family_meta["impls"].items():
        print(f"Generating implementation target for {family_key}.{impl_key}")
        target_inner_lines = [f"set(IMPL_KEY {impl_key})"]
        impl_enable_by = impl_meta["enable_by"]
        impl_param_key = impl_meta["param"]
        impl_param_meta = family_meta["params"][impl_param_key]
        if overwrite_default_impl_enable_by and (
            get_default_impl(family_meta, impl_param_key)[0] == impl_key
        ):
            impl_enable_by = impl_param_meta["enable_by"]
        # Find source files
        srcpaths = [
            os.path.join("${IMPL_KEY}", path)
            for path in impl_meta["copies"]
            if os.path.splitext(path)[1] in SRC_FILE_EXTS
        ]
        target_inner_lines.append(
            f"add_library({impl_key} OBJECT {" ".join(srcpaths)})"
        )
        # Add compile options, include directories
        for scope in SCOPE_OPTIONS:
            compile_opts = get_impl_compile_opts(impl_meta, scope)
            if compile_opts:
                target_inner_lines.append(
                    f"target_compile_options({impl_key} {scope.upper()} {" ".join(compile_opts)})"
                )
            include_dirs = get_impl_include_dirs(impl_meta, scope)
            if include_dirs:
                target_inner_lines.append(
                    f"target_include_directories({impl_key} {scope.upper()} {" ".join(include_dirs)})"
                )
            link_libs = get_impl_link_libs(impl_meta, scope)
            if link_libs:
                target_inner_lines.append(
                    f"target_link_libraries({impl_key} {scope.upper()} {" ".join(link_libs)})"
                )
        # CUDA Architecture if specified
        cuda_arch = impl_meta.get("cuda_arch", None)
        if cuda_arch:
            target_inner_lines.append(
                f"set_property(TARGET {impl_key} PROPERTY CUDA_ARCHITECTURES {cuda_arch})"
            )
        # Aggregate objects to local obj variable
        target_inner_lines.append(
            f"set({local_obj} ${{{local_obj}}} $<TARGET_OBJECTS:{impl_key}>)"
        )
        target = f"""\
if({impl_enable_by})
{"\n".join(target_inner_lines)}
endif()"""
        impl_targets.append(target)

    return common_targets + impl_targets


def generate_kem_cmake(
    kem_dir: str, kem_key: str, kem: dict, autoformat: bool = True
) -> str:
    """Generate the family-level CMakeLists.txt file for the input KEM scheme

    Each family-level list file (e.g. src/kem/ml_kem/CMakeLists.txt) exports a
    cmake variable (e.g. ML_KEM_OBJS) that contains the compiled objects from
    that family.

    :param kem_dir: path to the family-level subdirectory, such as
        LIBOQS_DIR/src/kem/ml_kem
    :param kem_key: the family key of the KEM scheme
    :param kem: the content in build file under the family key
    :param autoformat: format the generated list file with gersemi
    :return: path to the family-level cmake list file
    """
    local_obj = f"_{kem_key}_OBJS".upper()
    export_obj = f"{kem_key}_OBJS".upper()

    targets = generate_family_cmake_targets(kem_key, kem, local_obj)
    targets = "\n\n".join(targets)

    data = f"""\
# {SPDX_LICENSE_IDENTIFIER}
# This file is generated by OQS Builder ({__name__}.{currentframe_funcname()})

set({local_obj} "")

{targets}

set({export_obj} ${{{local_obj}}} PARENT_SCOPE)
"""

    cmake_path = os.path.join(kem_dir, "CMakeLists.txt")
    with open(cmake_path, "w") as f:
        f.write(data)
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
    kem_dir: str, kem_key: str, kem_meta: dict, autoformat: bool = True
) -> str:
    """Generate the family-level header file, such as
    LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem.h.

    Return the path to the generated header file.
    """
    header_path = os.path.join(kem_dir, kem_meta["header"])

    param_fragments = []
    for param_key, param_meta in kem_meta["params"].items():
        param_enable_by = param_meta["enable_by"]
        fragment = f"""\
#define OQS_KEM_{param_key}_length_public_key {param_meta["pklen"]}
#define OQS_KEM_{param_key}_length_secret_key {param_meta["sklen"]}
#define OQS_KEM_{param_key}_length_ciphertext {param_meta["ctlen"]}
#define OQS_KEM_{param_key}_length_shared_secret {param_meta["sslen"]}
#define OQS_KEM_{param_key}_length_keypair_seed {param_meta["keypair_seedlen"]}
#define OQS_KEM_{param_key}_length_encaps_seed {param_meta["encap_seedlen"]}
OQS_KEM *OQS_KEM_{param_key}_new(void);
OQS_API OQS_STATUS OQS_KEM_{param_key}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_{param_key}_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_{param_key}_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_{param_key}_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_{param_key}_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);"""
        fragment = f"""\
#if defined({param_enable_by})
{fragment}
#endif /* {param_enable_by} */
"""
        param_fragments.append(fragment)

    body = "\n".join(param_fragments)
    header = f"""\
// {SPDX_LICENSE_IDENTIFIER}
// This file is generated by OQS Builder ({__name__}.{currentframe_funcname()})

#ifndef OQS_KEM_{kem_key.upper()}_H
#define OQS_KEM_{kem_key.upper()}_H

#include <oqs/oqs.h>

{body}

#endif /* !OQS_KEM_{kem_key.upper()}_H */
"""
    with open(header_path, "w") as f:
        f.write(header)
    if autoformat:
        format_with_astyle(header_path)
    return header_path


def render_oqs_kem_new_impl(
    param_key: str, alg_version: str, nist_level: int, ind_cca: bool
) -> str:
    """Render the implementation of the function

    OQS_KEM *OQS_KEM_{param_key}_new(void) { /* ... */ }
    """
    assert nist_level in NIST_LEVELS, f"Invalid NIST level {nist_level}"
    code = OQS_KEM_NEW_IMPL.format(
        param_key=param_key,
        alg_version=alg_version,
        nist_level=nist_level,
        ind_cca="true" if ind_cca else "false",
    )
    return code


def render_kem_impl_extern_decl(
    keypair: str,
    keypair_derand: str | None,
    enc: str,
    enc_derand: str | None,
    dec: str,
    impl_enable_by: str | None,
    arch_enable_by: str | None,
) -> str:
    """Render a single set of external API declarations for the input impl"""
    decl_lines = [
        f"""\
extern int {keypair}(uint8_t *pk, uint8_t *sk);"""
    ]
    if keypair_derand:
        decl_lines.append(
            f"""\
extern int {keypair_derand}(uint8_t *pk, uint8_t *sk, const uint8_t *seed);"""
        )
    decl_lines.append(
        f"""\
extern int {enc}(uint8_t *ct, uint8_t *ss, const uint8_t *pk);"""
    )
    if enc_derand:
        decl_lines.append(
            f"""\
extern int {enc_derand}(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *seed);"""
        )
    decl_lines.append(
        f"""\
extern int {dec}(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);"""
    )

    decl = "\n".join(decl_lines)
    if impl_enable_by:
        decl = f"""\
#if defined({impl_enable_by})
{decl}
#endif /* {impl_enable_by} */"""
    if arch_enable_by:
        decl = f"""\
#if defined({arch_enable_by})
{decl}
#endif /* {arch_enable_by} */"""
    return decl


def render_kem_extern_decl(family_meta: dict, param_key: str) -> str:
    """Render and return fragment of source code that includes all external API
    declarations for the specified parameter set.

    The "enable_by" flag of each parameter set's default implementation duplicates
    the "enable_by" flag of the parameter set, so it will be ignored. The "enable_by"
    flag of each non-default implementation will surround this implementation's
    declarations. Additionally, if the implementatino has a non-standard architecture
    such as CUDA or ICICLE, this implementation's declarations will also be surrounded
    by architecture flags.

    ```c
    #if defined(OQS_ENABLE_KEM_ml_kem_768)

    /* default impl: no additional guards */
    extern int extern int PQCP_MLKEM_NATIVE_MLKEM768_C_XXX(...);

    /* non-default impl, "standard" architecture (one of ref, x86, or aarch64) */
    #if defined(OQS_ENABLE_KEM_ml_kem_768_x86_64)
    extern int PQCP_MLKEM_NATIVE_MLKEM768_X86_64_XXX(...);
    #endif /* OQS_ENABLE_KEM_ml_kem_768_x86_64 */

    /* non-default impl, "special" architecture */
    #if defined(OQS_USE_CUPQC)
    #if defined(OQS_ENABLE_KEM_ml_kem_768_cuda)
    extern int cupqc_ml_kem_768_XXX(...);
    #endif /* OQS_ENABLE_KEM_ml_kem_768_cuda */
    #endif /* OQS_USE_CUPQC */

    #endif /* OQS_ENABLE_KEM_ml_kem_768 */
    ```
    """
    _, default_impl_meta = get_default_impl(family_meta, param_key)
    default_decl = render_kem_impl_extern_decl(
        default_impl_meta["keypair"],
        default_impl_meta.get("keypair_derand", None),
        default_impl_meta["enc"],
        default_impl_meta.get("enc_derand", None),
        default_impl_meta["dec"],
        None,
        None,
    )
    addtl_decl_frags = []
    for _, impl_meta in get_impls(family_meta, param_key, exclude_default=True):
        arch_enable_by = impl_meta["arch"].get("enable_by", None)
        frag = render_kem_impl_extern_decl(
            impl_meta["keypair"],
            impl_meta.get("keypair", None),
            impl_meta["enc"],
            impl_meta.get("enc_derand", None),
            impl_meta["dec"],
            impl_meta["enable_by"],
            arch_enable_by,
        )
        addtl_decl_frags.append(frag)
    addtl_decl = "\n\n".join(addtl_decl_frags)
    decl = f"""\
{default_decl}

{addtl_decl}"""
    return decl

def render_oqs_kem_keypair_derand_impl(kem_meta: dict, param_key: str) -> str:
    """Render the implementation of the `keypair_derand` API for the input KEM
    parameter set.

    Not all KEM families support deterministic key generation. Family support for
    deterministic key generation is indicated by the field `derandomized_keypair`
    under each family. If the family does not support deterministic key generation,
    then the implementation should return an error.

    :param kem_meta: KEM family's metadata
    :param param_key: Input parameter set's key
    """
    # FIX: implement this
    fragment = f"""\
OQS_API OQS_STATUS OQS_KEM_{param_key}_keypair_derand(uint8_t *public_key,
                                                      uint8_t *secret_key, 
                                                      const uint8_t *seed) {{
    /* TODO: implement this */
}}"""
    return fragment

def generate_kem_source(
    kem_dir: str,
    kem_key: str,
    kem_meta: dict,
    param_key: str,
    param_meta: dict,
    autoformat: bool = True,
) -> str:
    """Generate a single family-level source file for the specified parameter set.
    Return the path to the generated file
    """
    source_path = os.path.join(kem_dir, f"kem_{param_key}.c")
    # FIX: implement these
    oqs_kem_new = render_oqs_kem_new_impl(
        param_key, kem_meta["version"], param_meta["nist_level"], param_meta["ind_cca"]
    )
    extern_api_decl = render_kem_extern_decl(kem_meta, param_key)
    keypair_derand = render_oqs_kem_keypair_derand_impl(kem_meta, param_key)
    keypair = ""
    encaps_derand = ""
    encaps = ""
    decaps = ""
    source = f"""\
// {SPDX_LICENSE_IDENTIFIER}
// This file is generated by OQS Builder ({__name__}.{currentframe_funcname()})

#include <stdlib.h>

#include <oqs/kem_{kem_key}.h>

#if defined(OQS_ENABLE_KEM_{param_key})
{oqs_kem_new}

{extern_api_decl}

{keypair_derand}

{keypair}

{encaps_derand}

{encaps}

{decaps}
#endif /* OQS_ENABLE_KEM_{param_key} */
"""
    print(f">>>>>>>> {source_path}")
    print(source)
    print("<<<<<<<<")
    raise NotImplementedError(f"What to write to {source_path}?")
    if autoformat:
        format_with_astyle(source_path)
    return source_path


def generate_kem_sources(
    kem_dir: str, kem_key: str, kem_meta: dict, autoformat: bool = True
) -> list[str]:
    """Generate the family-level source file(s), such as
    LIBOQS_DIR/src/kem/ml_kem/kem_ml_kem_<512|768|1024>.c
    """
    source_paths = []
    for param_key, param_meta in kem_meta["params"].items():
        source_path = generate_kem_source(
            kem_dir, kem_key, kem_meta, param_key, param_meta, autoformat
        )
        source_paths.append(source_path)
    return source_paths
