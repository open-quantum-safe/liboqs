from argparse import ArgumentParser
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


if __name__ == "__main__":
    argparser = ArgumentParser()
    argparser.add_argument("--version", action="store_true", help="Show version")
    # TODO: add argument to specify patch dir
    argparser.add_argument(
        "-f",
        "--file",
        type=str,
        help="path to copy_from_upstream.yml",
    )
    argparser.add_argument("cmd", nargs="?", choices=["copy"])
    args = argparser.parse_args()

    if args.version:
        print_version()
    elif args.cmd == "copy":
        if not args.file:
            print(
                f"{__package__}: error: the following arguments are required -f/--file"
            )
            exit(1)
        with open(args.file, mode="r", encoding="utf-8") as f:
            instructions = yaml.safe_load(f)
        print(f"Successfully loaded {args.file}")
        upstreams: list[dict[str, str]] = instructions["upstreams"]
        # NOTE: on MacOS, TemporaryDirectory() is created under /var, which is
        #   a symlink to private/var, but git considers tracking symlinks to be
        #   unsafe and will refuse to run. Hence we need to use a base directory
        #   that has not symlink component, such as LIBOQS_DIR. Fortunately,
        #   all of TemporaryDirectory's automatic cleanup still works.
        with TemporaryDirectory(dir=LIBOQS_DIR) as tempdir:
            for upstream in upstreams:
                clone_remote_repo(
                    tempdir,
                    upstream["name"],
                    upstream["git_url"],
                    commit=upstream.get("git_commit", None),
                    branch_or_tag=upstream.get("git_branch", None),
                )
                # TODO: apply patches
    else:
        argparser.print_help()
