import enum
import os
import subprocess


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
    if not gitdir:
        gitdir = os.path.join(dstdir, ".git")
    if not os.path.isdir(gitdir):
        raise FileNotFoundError(f"{gitdir} is not a valid .git directory")
    if not worktree:
        worktree = dstdir
    if not os.path.isdir(worktree):
        raise FileNotFoundError(f"{worktree} is not a valid git work tree")
    if not directory:
        directory = dstdir
    if not os.path.isdir(directory):
        raise FileNotFoundError(f"{directory} is not a valid directory")
    if isinstance(patches, list):
        if len(patches) == 0:
            return
        for patch in patches:
            if not os.path.isfile(patch):
                raise FileNotFoundError(f"{patch} is not a valid patch file")
    else:
        if not os.path.isfile(patches):
            raise FileNotFoundError(f"{patches} is not a valid patch file")

    if not commit_msg:
        if isinstance(patches, list):
            patch_names: list[str] = []
            for patch in patches:
                _, patch_filename = os.path.split(patch)
                patch_name, _ = os.path.splitext(patch_filename)
                patch_names.append(patch_name)
            commit_msg = f"Applied {', '.join(patch_names)}"
        else:
            _, patch_filename = os.path.split(patches)
            patch_name, _ = os.path.splitext(patch_filename)
            commit_msg = f"Applied {patch_name}"

    git_apply_cmd = (
        ["git", "--git-dir", gitdir, "--work-tree", worktree]
        + ["apply", "--unsafe-paths", "--verbose", "--whitespace", "fix"]
        + ["--directory", directory]
    )
    if isinstance(patches, list):
        git_apply_cmd += patches
    else:
        git_apply_cmd += [patches]
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
