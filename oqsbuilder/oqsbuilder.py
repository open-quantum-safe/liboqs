import os
import subprocess


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
    patch: str,
    gitdir: str | None = None,
    worktree: str | None = None,
    directory: str | None = None,
    commit_after_apply: bool = True,
    commit_msg: str | None = None,
    dryrun: bool = False,
):
    """Apply a patch to the specified git repository

    :param dstdir: path to the git repository on which the patch will be applied
    :param patch: path to the patch file
    :param gitdir: path to the .git directory, defaults to {dstdir}/.git
    :param worktree: path to the worktree, defaults to {dstdir}
    :param directory: prepend to filenames in the patch file, see "git apply --directory=<root>", defaults to {dstdir}
    :param commit_after_apply: if True, commit the changes after applying the patch
    :param commit_msg: specify a commit message if commit_after_apply, defaults to "applied {patch}"
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
    if not os.path.isfile(patch):
        raise FileNotFoundError(f"{patch} is not a valid patch file")
    if not commit_msg:
        _, patch_filename = os.path.split(patch)
        patch_name, _ = os.path.splitext(patch_filename)
        commit_msg = f"Applied {patch_name}"

    commands = [
        ["git", "--git-dir", gitdir, "--work-tree", worktree]
        + ["apply", "--unsafe-paths", "--verbose", "--whitespace", "fix"]
        + ["--directory", directory, patch]
    ]
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
