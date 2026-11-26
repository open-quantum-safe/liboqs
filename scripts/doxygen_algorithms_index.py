# SPDX-License-Identifier: MIT
"""Write the Doxygen index page for the algorithm datasheets.

scripts/run_doxygen.sh calls this before running Doxygen. The page has the
label "algorithms", so links to docs/algorithms resolve to it, and it lists
every datasheet under docs/algorithms as a subpage. This groups the
datasheets under a single "Algorithm datasheets" entry in the navigation
tree.

Usage: doxygen_algorithms_index.py <liboqs root> <output .md file>
"""

import os
import sys

from doxyfy_datasheet import page_label

SECTIONS = [
    ("kem", "Key encapsulation mechanisms"),
    ("sig", "Signature schemes"),
    ("sig_stfl", "Stateful signature schemes"),
]


def datasheet_names(liboqs_dir: str, datasheet_type: str):
    """Datasheet names of one type, sorted. A page named after the type itself
    (docs/algorithms/sig_stfl/sig_stfl.md) is an overview and comes first."""
    directory = os.path.join(liboqs_dir, "docs", "algorithms", datasheet_type)
    names = (os.path.splitext(f)[0] for f in os.listdir(directory) if f.endswith(".md"))
    return sorted(names, key=lambda name: (name != datasheet_type, name))


def render_index(liboqs_dir: str) -> str:
    lines = [
        "# Algorithm datasheets {#algorithms}",
        "",
        "These pages are the datasheets in `docs/algorithms`. Each algorithm "
        "family has one page with its upstream sources, support tier, "
        "parameter sets and implementation characteristics.",
    ]
    for datasheet_type, title in SECTIONS:
        lines += ["", f"## {title} {{#algorithms-{datasheet_type}}}", ""]
        for name in datasheet_names(liboqs_dir, datasheet_type):
            lines.append(f"- \\subpage {page_label(datasheet_type, name)}")
    return "\n".join(lines) + "\n"


def main(argv):
    if len(argv) != 3:
        sys.stderr.write(__doc__)
        return 1
    with open(argv[2], "w", encoding="utf-8", newline="\n") as f:
        f.write(render_index(argv[1]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
