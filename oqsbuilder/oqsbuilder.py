import enum
import os
import shutil
import subprocess

import yaml

from oqsbuilder.templates import FAMILY_CMAKE_HEADER

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

    for primitive in [
        CryptoPrimitive.KEM,
        # FIX: uncomment this once sigs and stfl_sigs are filled in
        # CryptoPrimitive.SIG,
        # CryptoPrimitive.STFL_SIG,
    ]:
        for _, family in oqsbuild[primitive.get_oqsbuildfile_key()]["families"].items():
            for _, impl_meta in family["impls"].items():
                impl_copies = impl_meta["copies"]
                if isinstance(impl_copies, str):
                    impl_meta["copies"] = oqsbuild["copies"][impl_copies]

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
            os.path.join(impl_key, path)
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


def generate_kem_cmake(cmake_path: str, kem_key: str, kem: dict, dryrun: bool):
    """Generate the family-level CMakeLists.txt file for the input KEM scheme

    Each family-level list file (e.g. src/kem/ml_kem/CMakeLists.txt) exports a
    cmake variable (e.g. ML_KEM_OBJS) that contains the compiled objects from
    that family.

    :param cmake_path: the cmake list file will be written to this file
    :param kem_key: the family key of the KEM scheme
    :param kem: the content in build file under the family key
    """
    local_obj = f"_{kem_key}_OBJS".upper()
    export_obj = f"{kem_key}_OBJS".upper()

    targets = generate_family_cmake_targets(kem_key, kem, local_obj)
    targets = "\n\n".join(targets)

    data = f"""{FAMILY_CMAKE_HEADER}

set({local_obj} "")

{targets}

set({export_obj} ${{{local_obj}}} PARENT_SCOPE)
"""

    if dryrun:
        print(f">>>>>>>>> {cmake_path}:")
        print(data)
        return
    with open(cmake_path, "w") as f:
        f.write(data)
