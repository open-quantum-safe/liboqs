from argparse import ArgumentParser
import os
import sys
from tempfile import TemporaryDirectory
import yaml

import oqsbuilder
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
        with open(args.file, mode="r") as f:
            instructions = yaml.safe_load(f)
        print(f"Successfully loaded {args.file}")
        upstreams: list[dict[str, str]] = instructions["upstreams"]
        with TemporaryDirectory() as tempdir:
            for upstream in upstreams:
                clone_remote_repo(
                    tempdir,
                    upstream["name"],
                    upstream["git_url"],
                    commit=upstream.get("git_commit", None),
                    branch_or_tag=upstream.get("git_branch", None),
                )
            for name in os.listdir(tempdir):
                print(name)
    else:
        argparser.print_help()
