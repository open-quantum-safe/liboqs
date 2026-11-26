# SPDX-License-Identifier: MIT
"""Doxygen input filter for the algorithm datasheets under docs/algorithms.

Doxygen runs this script on each datasheet (see FILTER_PATTERNS in
docs/.Doxyfile) and reads the Doxygen-compatible version from stdout.
The datasheet on disk is never modified.

The filter makes three changes so that the datasheets can be Doxygen pages:

1. The page title gets an explicit label, algorithms-<type>-<name>
   (for example algorithms-kem-ml_kem), so that the index page and other
   pages can refer to it. Bold markers around the title are removed,
   because Doxygen shows page titles as plain text.
2. Doxygen labels are global, so every <a name="..."> anchor and every
   section heading gets the page label as a prefix (for example
   primary-source in kem/ml_kem.md becomes
   algorithms-kem-ml_kem-primary-source), and every in-page link (#...)
   is rewritten to match.
3. The <a name="..."> HTML anchors are replaced by \\anchor commands at the
   end of the line, which Doxygen accepts inside nested list items.

The number of lines is kept the same, so Doxygen warnings still point at
the right line of the original datasheet.
"""

import os
import re
import sys

ANCHOR_RE = re.compile(r'<a name="([^"]+)"></a>')
HEADING_RE = re.compile(r"^(#{2,6})\s+(.*?)\s*$")
INPAGE_LINK_RE = re.compile(r"\]\(#([^)\s]+)\)")
TITLE_EMPHASIS_RE = re.compile(r"^(?:\*\*|__)(.*)(?:\*\*|__)$")


def page_label(datasheet_type: str, name: str) -> str:
    """Label of the Doxygen page for a datasheet, e.g. algorithms-kem-ml_kem."""
    return f"algorithms-{datasheet_type}-{name}"


def github_slug(heading: str) -> str:
    """Anchor that GitHub generates for a markdown heading."""
    slug = heading.strip().lower()
    slug = re.sub(r"[^\w\- ]", "", slug)
    return slug.replace(" ", "-")


def datasheet_type_and_name(path: str):
    """Return ("kem", "ml_kem") for .../docs/algorithms/kem/ml_kem.md."""
    directory, filename = os.path.split(os.path.abspath(path))
    return os.path.basename(directory), os.path.splitext(filename)[0]


def doxyfy_datasheet(text: str, datasheet_type: str, name: str) -> str:
    lines = text.split("\n")

    # Collect every in-page target defined in this datasheet: HTML anchors
    # and the GitHub-style ids of its section headings.
    targets = set()
    for line in lines:
        targets.update(ANCHOR_RE.findall(line))
        match = HEADING_RE.match(line)
        if match:
            targets.add(github_slug(match.group(2)))

    def prefixed(target: str) -> str:
        return f"{page_label(datasheet_type, name)}-{target}"

    def rewrite_link(match):
        target = match.group(1)
        if target in targets:
            return f"](#{prefixed(target)})"
        return match.group(0)

    out = []
    title_done = False
    for line in lines:
        if not title_done and line.startswith("# "):
            # Doxygen shows page titles as plain text, so drop any bold
            # markers (e.g. "# **Stateful Hash Based Signatures**").
            title = TITLE_EMPHASIS_RE.sub(r"\1", line[2:].strip())
            line = f"# {title} {{#{page_label(datasheet_type, name)}}}"
            title_done = True
        else:
            match = HEADING_RE.match(line)
            if match:
                slug = github_slug(match.group(2))
                line = f"{match.group(1)} {match.group(2)} {{#{prefixed(slug)}}}"
        anchors = ANCHOR_RE.findall(line)
        if anchors:
            # Move the anchors to the end of the line so that the text
            # after them (often a ':') does not become part of the label.
            line = ANCHOR_RE.sub("", line).rstrip()
            line += "".join(f" \\anchor {prefixed(a)}" for a in anchors)
        line = INPAGE_LINK_RE.sub(rewrite_link, line)
        out.append(line)
    return "\n".join(out)


def main(argv):
    if len(argv) != 2 or not os.path.isfile(argv[1]):
        sys.stderr.write("Usage: doxyfy_datasheet.py <datasheet.md>\n")
        return 1
    datasheet_type, name = datasheet_type_and_name(argv[1])
    with open(argv[1], encoding="utf-8") as f:
        text = f.read()
    sys.stdout.reconfigure(encoding="utf-8", newline="\n")
    sys.stdout.write(doxyfy_datasheet(text, datasheet_type, name))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
