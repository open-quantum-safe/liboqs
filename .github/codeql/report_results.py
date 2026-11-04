#!/usr/bin/env python3
# SPDX-License-Identifier: MIT

import json
import sys

# Report the results of liboqs's custom CodeQL queries, failing if there are any.

def escape(text):
    # Characters with a special meaning in workflow command messages
    return text.replace('%', '%25').replace('\r', '%0D').replace('\n', '%0A')


def main(sarif_files):
    count = 0
    for sarif_file in sarif_files:
        with open(sarif_file) as f:
            sarif = json.load(f)
        for run in sarif['runs']:
            for result in run.get('results', []):
                location = result['locations'][0]['physicalLocation']
                path = location['artifactLocation']['uri']
                line = location['region']['startLine']
                message = f"[{result['ruleId']}] {result['message']['text']}"
                print(f"::error file={path},line={line}::{escape(message)}")
                count += 1

    print(f"{count} result(s) from liboqs CodeQL queries")
    return 1 if count else 0


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("usage: report_results.py SARIF_FILE...")
    sys.exit(main(sys.argv[1:]))
