# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import sys
import glob

# Check if liboqs contains any non-namespaced global symbols
# See https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for function naming conventions

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_namespace():
    liboqs = glob.glob(helpers.get_current_build_dir_name()+'/lib/liboqs.*')[0]
    if liboqs == helpers.get_current_build_dir_name()+'/lib/liboqs.dylib':
        out = helpers.run_subprocess(
            ['nm', '-g', liboqs]
        )
    elif liboqs == helpers.get_current_build_dir_name()+'/lib/liboqs.so':
        out = helpers.run_subprocess(
            ['nm', '-D', liboqs]
        )
    else:
        out = helpers.run_subprocess(
            ['nm', '-g', liboqs]
        )

    lines = out.strip().split("\n")
    symbols = []
    for line in lines:
        if ' T ' in line or ' D ' in line or ' S ' in line:
            symbols.append(line)

    # ideally this would be just ['oqs', 'pqclean'], but contains exceptions (e.g., providing compat implementations of unavailable platform functions)
    namespaces = ['oqs', 'pqclean', 'keccak', 'pqcrystals', 'pqmayo', 'init', 'fini', 'seedexpander', '__x86.get_pc_thunk', 'libjade', 'jade', '__jade', '__jasmin_syscall', 'pqcp', 'pqov', '_snova', 'sha3', 'slh', 'sha2', 'shake', 'hash', 'openssh', 'hqc', 'pqchqc']
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

@helpers.filtered_test
@pytest.mark.skipif(not(sys.platform.startswith("linux")), reason="Only supported on Linux")
@pytest.mark.skipif(not(os.path.exists(helpers.get_current_build_dir_name()+'/lib/liboqs.so')), reason="Only supported on builds with a shared library")
def test_non_executable_stack():
    liboqs = helpers.get_current_build_dir_name()+'/lib/liboqs.so'
    out = helpers.run_subprocess(
        ['readelf', '--wide', '--segments', liboqs]
    )
    lines = out.strip().split("\n")
    for line in lines:
        if "GNU_STACK" in line:
            chunks = line.strip().split()
            flags = chunks[6]
            assert(flags == 'RW')

# On Windows, OQS_API must not expand to __declspec(dllexport) in a static build:
# that leaves /EXPORT: linker directives in oqs.lib, so anything linking it
# re-exports the entire liboqs API.

@helpers.filtered_test
@pytest.mark.skipif(not(sys.platform.startswith("win")), reason="Only supported on Windows")
def test_static_lib_does_not_export_api():
    build_dir = helpers.get_current_build_dir_name()
    with open(os.path.join(build_dir, 'include', 'oqs', 'oqsconfig.h')) as fh:
        if any(line.startswith('#define BUILD_SHARED_LIBS') for line in fh):
            pytest.skip('Only applies to builds with a static library')

    libs = glob.glob(os.path.join(build_dir, 'lib', '**', '*.lib'), recursive=True)
    assert any(lib.endswith('oqs.lib') for lib in libs), "Unable to find oqs.lib under {}".format(os.path.join(build_dir, 'lib'))

    # MSVC stores linker directives as plain ASCII in the .drectve section,
    # so they can be found without dumpbin being on PATH.
    exporting = []
    for lib in libs:
        with open(lib, 'rb') as fh:
            if b'/export:oqs_' in fh.read().lower():
                exporting.append(lib)

    if len(exporting) > 0:
        for lib in exporting:
            print("Static library re-exports the liboqs API: {}".format(lib))

    assert(len(exporting) == 0)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

