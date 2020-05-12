# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys

# Check if liboqs contains any non-namespaced global symbols
# See https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for function naming conventions

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_namespace(use_liboqs_so):
    if use_liboqs_so:
        out = helpers.run_subprocess(
            ['nm', '-D', 'build/lib/liboqs.so']
        )
    else:
        out = helpers.run_subprocess(
            ['nm', '-g', 'build/lib/liboqs.a']
        )

    lines = out.strip().split("\n")
    symbols = []
    for line in lines:
        if ' T ' in line or ' D ' in line or ' S ' in line:
            symbols.append(line)

    # ideally this would be just ['oqs', 'pqclean']
    namespaces = ['oqs', 'pqclean', 'keccak', 'aligned_alloc', 'aligned_free']
    non_namespaced = []

    for symbolstr in symbols:
        *_, symtype, symbol = symbolstr.split()
        if symtype in 'TR':
            is_namespaced = False
            for namespace in namespaces:
                if symbol.lower().startswith(namespace) or symbol.lower().startswith('_' + namespace):
                    is_namespaced = True
            if not(is_namespaced):
                non_namespaced.append(symbol)

    if len(non_namespaced) > 0:
        for symbol in non_namespaced:
            print("Non-namespaced symbol: {}".format(symbol))

    assert(len(non_namespaced) == 0)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

