#!/usr/bin/env python3
# SPDX-License-Identifier: MIT

import os

import tabulate
import yaml

YAML_EXTS = [".yaml", ".yml"]
ALG_SUPPORT_HEADER = [
    "Algorithm family",
    "Standardization status",
    "Primary implementation",
]
COMMIT_HASH_LEN = 7


def format_upstream_source(source: str) -> str:
    """For each YAML data sheet, the primary-upstream.source field contains some
    URL to the implementation. At this moment all URLs are links to GitHub, so
    we can format them as follows:

    <handle>/<repository>@<commit> if commit is available
    <handle>/<repository> otherwise
    with a link to the repository
    """
    # TODO: we might get GitLab or other non-GH link, but we will figure it out when it comes
    prefix = "https://github.com/"
    if not prefix in source:
        raise ValueError(f"Non-GitHub source {source}")
    url_start = source.find(prefix)
    # NOTE: split with no argument will split with all whitespaces
    url = source[url_start:].split()[0]
    # example: ["PQClean", "PQClean", "commit", "1eacfdaf..."]
    tokens = url[len(prefix) :].split("/")
    handle, repo = tokens[0], tokens[1]
    output = f"{handle}/{repo}"
    if "commit/" in url:
        commit = tokens[3][:COMMIT_HASH_LEN]
        output += f"@{commit}"
    return f"[{output}]({url})"


def render_alg_support_tbl(doc_dir: str) -> str:
    """Render a markdown table summarizing the algorithms described by YAML data
    sheets stored in the specified doc directory
    """
    yaml_paths = [
        os.path.abspath(os.path.join(doc_dir, filepath))
        for filepath in os.listdir(doc_dir)
        if os.path.splitext(filepath)[1].lower() in YAML_EXTS
    ]
    rows = [ALG_SUPPORT_HEADER]
    for yaml_path in yaml_paths:
        with open(yaml_path) as f:
            algdata = yaml.safe_load(f)
        alg_name = algdata["name"]
        dirname = "kem"
        if "sig/" in yaml_path:
            dirname = "sig"
        elif "sig_stfl/" in yaml_path:
            dirname = "sig_stfl"
        md_basename = os.path.splitext(os.path.split(yaml_path)[1])[0]
        md_url = f"./docs/algorithms/{dirname}/{md_basename}.md"
        std_status = algdata["standardization-status"]
        spec_url = algdata["spec-url"]
        primary_impl = format_upstream_source(algdata["primary-upstream"]["source"])
        rows.append(
            [
                f"[{alg_name}]({md_url})",
                f"[{std_status}]({spec_url})",
                primary_impl,
            ]
        )
    tbl = tabulate.tabulate(rows, tablefmt="pipe", headers="firstrow")
    return tbl


def update_readme(
    liboqs_dir: str,
):
    """Per liboqs/issues/2045, update README.md with an algorithm support table

    The algorithm support table is a summary of individual algorithms currently
    integrated into liboqs. The primary source of information are the various
    YAML files under docs/algorithms/<kem|sig|sig_stfl> directory. The table
    summarizes the following attributes:
    - Algorithm family (e.g. Kyber, ML-KEM)
    - Standardization status, with link to specification
    - Primary source of implementation
    - (WIP) Maintenance status
    """
    kem_doc_dir = os.path.join(liboqs_dir, "docs", "algorithms", "kem")
    kem_tbl = render_alg_support_tbl(kem_doc_dir)
    sig_doc_dir = os.path.join(liboqs_dir, "docs", "algorithms", "sig")
    sig_tbl = render_alg_support_tbl(sig_doc_dir)
    sig_stfl_doc_dir = os.path.join(liboqs_dir, "docs", "algorithms", "sig_stfl")
    sig_stfl_tbl = render_alg_support_tbl(sig_stfl_doc_dir)
    md_str = f"""### KEMs
{kem_tbl}

### Digital signatures
{sig_tbl}

### Stateful digital signatures
{sig_stfl_tbl}
"""
    readme_path = os.path.join(liboqs_dir, "README.md")
    fragment_start = "<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_START -->\n"
    fragment_end = "<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_END -->"
    with open(readme_path, "r") as f:
        readme = f.read()
        fragment_start_loc = readme.find(fragment_start) + len(fragment_start)
        fragment_end_loc = readme.find(fragment_end)
    with open(readme_path, "w") as f:
        f.write(readme[:fragment_start_loc])
        f.write(md_str)
        f.write(readme[fragment_end_loc:])
