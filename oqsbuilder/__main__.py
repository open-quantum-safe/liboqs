import os
import sys
import shutil
from tempfile import TemporaryDirectory
import yaml

import oqsbuilder
from oqsbuilder import LIBOQS_DIR
from oqsbuilder.oqsbuilder import (
    clone_remote_repo,
    git_apply,
    CryptoPrimitive,
    get_copies,
)


def print_version():
    """Print version and system info"""
    print(f"OQS Builder v{oqsbuilder.__version__}")
    print("PYTHONPATH:")
    for path in sys.path:
        print(f"-   {path}")


def copy_from_upstream(
    oqsbuildfile: str,
    patch_dir: str,
    upstream_parent_dir: str = LIBOQS_DIR,
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
    :param headless: True if running in a non-interactive environment
    """
    with open(oqsbuildfile, mode="r", encoding="utf-8") as f:
        instructions = yaml.safe_load(f)
    print(f"Successfully loaded {oqsbuildfile}")
    upstreams = instructions["upstreams"]
    with TemporaryDirectory(dir=upstream_parent_dir) as tempdir:
        for name, upstream in upstreams.items():
            upstream_dir = clone_remote_repo(
                tempdir,
                name,
                upstream["git_url"],
                commit=upstream.get("git_commit", None),
                branch_or_tag=upstream.get("git_branch", None),
            )
            patches: list[str] = [
                os.path.join(patch_dir, patch) for patch in upstream.get("patches", [])
            ]
            git_apply(upstream_dir, patches)
        print(f"SUCCESS: fetched {len(upstreams)} upstream repositories")

        kems = instructions["kems"]
        kems_dir = os.path.join(LIBOQS_DIR, "src", "kem")
        for kem_key, kem in kems["families"].items():
            kem_dir = os.path.join(kems_dir, kem_key)
            print(f"Integrating {kem_key} into {kem_dir}")
            for impl_key, impl in kem["impls"].items():
                upstream_key = impl["upstream"]
                upstream_dir = os.path.join(tempdir, upstream_key)
                impl_dir = os.path.join(kem_dir, impl_key)
                if not os.path.isdir(upstream_dir):
                    raise FileNotFoundError(
                        f"{kem_key}.{impl_key}'s upstream {upstream_key} not found"
                    )
                copies = get_copies(
                    instructions, CryptoPrimitive.KEM, kem_key, impl_key
                )
                for dst, src in copies.items():
                    src = os.path.join(upstream_dir, src)
                    dst = os.path.join(impl_dir, dst)
                    dst_parent_dir = os.path.split(dst)[0]
                    if not os.path.isdir(dst_parent_dir):
                        print(f"mkdir -p {dst_parent_dir}")
                        os.makedirs(dst_parent_dir)
                    # use shutil instead of subprocess.run(["cp", ...]) for OS portability
                    shutil.copyfile(src, dst)
                print(f"Copied {len(impl["copies"])} files into {impl_dir}")


if __name__ == "__main__":
    print_version()
    buildfile = os.path.join(LIBOQS_DIR, "oqsbuilder", "oqsbuildfile.yml")
    patch_dir = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
    copy_from_upstream(buildfile, patch_dir)
