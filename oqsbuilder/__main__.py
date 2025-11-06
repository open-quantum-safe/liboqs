import os
import sys
from tempfile import TemporaryDirectory
import yaml

import oqsbuilder
from oqsbuilder import LIBOQS_DIR
from oqsbuilder.oqsbuilder import clone_remote_repo


def print_version():
    """Print version and system info"""
    print(f"OQS Builder v{oqsbuilder.__version__}")
    print("PYTHONPATH:")
    for path in sys.path:
        print(f"-   {path}")


def copy_from_upstream(
    oqsbuildfile: str, patch_dir: str, upstream_parent_dir: str = LIBOQS_DIR
):
    """Copy implementations from upstream

    on MacOS, TemporaryDirectory() is created under /var, which is
    a symlink to private/var, but git considers tracking symlinks to be
    unsafe and will refuse to run. Hence we need to use a base directory
    that has not symlink component, such as LIBOQS_DIR. Fortunately,
    all of TemporaryDirectory's automatic cleanup still works.

    :param oqsbuildfile: path to copy_from_upstream.yml
    :param patch_dir: path to a directory hosting the patch files for upstream
    :param upstream_parent_dir: upstream repositories will be cloned into
        a temporary subdirectory under this directory
    """
    with open(oqsbuildfile, mode="r", encoding="utf-8") as f:
        instructions = yaml.safe_load(f)
    print(f"Successfully loaded {oqsbuildfile}")
    upstreams: list[dict[str, str]] = instructions["upstreams"]
    with TemporaryDirectory(dir=upstream_parent_dir) as tempdir:
        for upstream in upstreams:
            clone_remote_repo(
                tempdir,
                upstream["name"],
                upstream["git_url"],
                commit=upstream.get("git_commit", None),
                branch_or_tag=upstream.get("git_branch", None),
            )
            # TODO: apply patches


if __name__ == "__main__":
    print_version()
    buildfile = os.path.join(LIBOQS_DIR, "oqsbuilder", "oqsbuildfile.yml")
    patch_dir = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
    copy_from_upstream(buildfile, patch_dir)
