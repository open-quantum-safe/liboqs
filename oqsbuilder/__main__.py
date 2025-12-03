import os
import sys
from tempfile import TemporaryDirectory

import oqsbuilder
from oqsbuilder import LIBOQS_DIR
from oqsbuilder.oqsbuilder import (
    CryptoPrimitive,
    copy_copies,
    generate_kem_cmake,
    load_oqsbuildfile,
    fetch_upstreams,
    generate_kem_header,
    generate_kem_sources,
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
    templates_dir: str,
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
    :param templates_dir: path to a directory containing the Jinja2 templates
        used to generate CMakeLists.txt files, source files, and header files
    :param headless: True if running in a non-interactive environment
    """
    assert os.path.isfile(oqsbuildfile), f"{oqsbuildfile} is not a valid file"
    assert os.path.isdir(patch_dir), f"{patch_dir} is not a valid directory"
    assert os.path.isdir(templates_dir), f"{templates_dir} is not a valid directory"
    assert os.path.isdir(
        upstream_parent_dir
    ), f"{upstream_parent_dir} is not a valid directory"

    oqsbuild = load_oqsbuildfile(oqsbuildfile)
    with TemporaryDirectory(dir=upstream_parent_dir) as tempdir:
        upstream_dirs = fetch_upstreams(oqsbuild, tempdir, patch_dir)

        kems = oqsbuild[CryptoPrimitive.KEM.get_oqsbuildfile_key()]
        kems_dir = os.path.join(
            LIBOQS_DIR, "src", CryptoPrimitive.KEM.get_subdirectory_name()
        )
        for kem_key, kem in kems["families"].items():
            kem_dir = os.path.join(kems_dir, kem_key)
            print(f"Integrating {kem_key} into {kem_dir}")
            for impl_key, impl in kem["impls"].items():
                impl_dir = os.path.join(kem_dir, impl_key)
                copy_copies(impl["copies"], upstream_dirs[impl["upstream"]], impl_dir)
            kem_cmake_path = generate_kem_cmake(kem_dir, kem_key, kem)
            kem_header_path = generate_kem_header(kem_dir, kem_key, kem, templates_dir)
            kem_src_paths = generate_kem_sources(kem_dir, kem_key, kem, templates_dir)


if __name__ == "__main__":
    print_version()
    buildfile = os.path.join(LIBOQS_DIR, "oqsbuilder", "oqsbuildfile.yml")
    patch_dir = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")
    templates_dir = os.path.join(LIBOQS_DIR, "oqsbuilder", "templates")
    copy_from_upstream(buildfile, patch_dir, templates_dir)
