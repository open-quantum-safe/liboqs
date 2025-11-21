import enum
import os
import shutil
import subprocess
from typing import Dict

from oqsbuilder.templates import FAMILY_CMAKE_HEADER

# TODO: make it into a proper schema with schema validation
OQSBuild = Dict
UpstreamKey = UpstreamPath = str


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
) -> UpstreamPath:
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
    oqsbuild: OQSBuild, upstream_parent_dir: str, patch_dir: str
) -> dict[UpstreamKey, UpstreamPath]:
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

def add_obj_library(libname: str, impl_meta: dict) -> str:
    """Given implementation metadata, return a CMake fragment that builds the 
    implementation into an object library
    """
    # TODO: implement this
    return ""

def add_objs(kem_key: str, kem: dict) -> list[str]:
    """Return a list of cmake "add_library" sections that build individual
    implementations into an object
    """
    targets = []

    for param_key, param in kem["params"].items():
        # TODO: default impl is special for two reasons:
        # - default impl's enable_by is overwritten by parameter set's enable_by
        # - default impl's obj library also contains a parameter-set-level .c file,
        #   for example: src/kem/ml_kem/kem_ml_kem_512.c
        default_impl_key, default_impl = get_default_impl(kem, param_key)
        default_impl["enable_by"] = param["enable_by"]
        targets.append(add_obj_library(default_impl_key, default_impl))
        for impl_key, impl in get_impls(kem, param_key, True):
            targets.append(add_obj_library(impl_key, impl))

    return targets


def generate_kem_cmake(cmake_path: str, kem_key: str, kem: dict, dryrun: bool):
    """Generate the family-level CMakeLists.txt file for the input KEM scheme

    :param cmake_path: the cmake list file will be written to this file
    :param kem_key: the family key of the KEM scheme
    :param kem: the content in build file under the family key
    """
    local_obj = f"_{kem_key}_OBJS".upper()
    export_obj = f"{kem_key}_OBJS".upper()

    targets = add_objs(kem_key, kem)
    targets = "\n\n".join(targets)

    data = f"""{FAMILY_CMAKE_HEADER}

set({local_obj} "")

{targets}

set({export_obj} ${{{local_obj}}} PARENT_SCOPE)
"""

    if dryrun:
        print(f">>> {cmake_path}:")
        print(data)
        return
    with open(cmake_path, "w") as f:
        f.write(data)
