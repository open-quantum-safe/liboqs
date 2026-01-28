import os
import sys
from tempfile import TemporaryDirectory

from oqsbuilder import (
    LIBOQS_DIR,
    LIBOQS_JINJA_TEMPLATES_DIR,
    __version__ as oqsbuilderversion,
)
from oqsbuilder.oqsbuilder import OQSBuilder


def print_version():
    """Print version and system info"""
    print(f"OQS Builder v{oqsbuilderversion}")
    print("PYTHONPATH:")
    for path in sys.path:
        print(f"-   {path}")


def copy_from_upstream(
    oqsbuildfile: str,
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
    :param upstream_parent_dir: upstream repositories will be cloned into
        a temporary subdirectory under this directory
    :param templates_dir: path to a directory containing the Jinja2 templates
        used to generate CMakeLists.txt files, source files, and header files
    :param headless: True if running in a non-interactive environment
    """
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


if __name__ == "__main__":
    print_version()
    buildfile = os.path.join(LIBOQS_DIR, "oqsbuilder", "oqsbuildfile.yml")
    copy_from_upstream(buildfile, LIBOQS_JINJA_TEMPLATES_DIR)
