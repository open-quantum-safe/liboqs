# SPDX-License-Identifier: MIT

import fnmatch
import helpers
import os
import pytest
import re
import sys

# Ensure every key-exchange algorithm in the code
# is mentioned in the documentation.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_datasheet_kem(kem_name):
    helpers.run_subprocess(
        ['grep', '-r', kem_name, 'docs/algorithms']
    )

# Ensure every signature algorithm in the code
# is mentioned in the documentation.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_datasheet_sig(sig_name):
    # skip extmu variants
    if "-extmu" in sig_name:
        pytest.skip('extmu variants do not have separate datasheets')
    helpers.run_subprocess(
        ['grep', '-r', sig_name, 'docs/algorithms']
    )

# Ensure astyle agrees with the formatting.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_style():

    result = helpers.run_subprocess(
        ['tests/run_astyle.sh']
    )
    assert 'Formatted' not in result

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_spdx():

    result = helpers.run_subprocess(
        ['tests/test_spdx.sh']
    )
    if len(result) != 0:
        print("The following files do not have proper SPDX-License-Identifier headers:")
        print(result)
        assert False

def test_memory_functions():
    c_h_files = []
    for path, _, files in os.walk('src'):
        c_h_files += [os.path.join(path, f) for f in files if f.endswith(('.c', '.h', '.fragment'))]

    memory_functions = ['free', 'malloc', 'calloc', 'realloc', 'strdup']
    okay = True

    for fn in c_h_files:
        with open(fn) as f:
            content = f.read()
            lines = content.splitlines()
            in_multiline_comment = False
            for no, line in enumerate(lines, 1):
                # Skip single-line comments
                if line.strip().startswith('//'):
                    continue
                # Check for start of multi-line comment
                if '/*' in line and not in_multiline_comment:
                    in_multiline_comment = True
                # Check for end of multi-line comment
                if '*/' in line and in_multiline_comment:
                    in_multiline_comment = False
                    continue
                # Skip lines inside multi-line comments
                if in_multiline_comment:
                    continue
                for func in memory_functions:
                    if re.search(r'\b{}\('.format(func), line) and not re.search(r'\b_{}\('.format(func), line):
                        if 'IGNORE memory-check' in line:
                            continue
                        okay = False
                        print(f"Suspicious `{func}` in {fn}:{no}:{line.strip()}")

    assert okay, ("Standard memory functions are used in some files. "
                  "These should be changed to OQS_MEM_* equivalents as appropriate. "
                  "If you are sure you want to use these functions in a particular spot, "
                  "add the comment '// IGNORE memory-check' on the line where the function occurs.")

# Ignore patterns to ignore false-positives in upstreams without the need to
# insert an `// IGNORE malloc-check`.
MALLOC_CHECK_IGNORED_DIRS = [
    'src/sig/cross/upcross_*',  # CROSS
    'src/sig_stfl/*/external',  # XMSS and LMS
]

# Ensure every OQS_MEM_{malloc,calloc,strdup,aligned_alloc} call is immediately
# followed by a null check (OQS_EXIT_IF_NULLPTR, "== NULL", "if (!ptr)", ...).
# Can add `// IGNORE malloc-check` to supress a false positive, or modify
# `MALLOC_CHECK_IGNORED_DIRS` above.
# TODO: this tests for common null-check patterns, but a stronger check would
# involve using an analyzer to track the real dataflow
def test_malloc_null_check():
    def is_allocation(line):
        # A call to one of the OQS_MEM_* allocators (excluding the wrappers' own declarations/definitions)
        return (re.search(r'\bOQS_MEM_(?:malloc|calloc|strdup|aligned_alloc)\(', line) is not None
                and re.search(r'\*\s*OQS_MEM_\w+\(', line) is None)

    def is_null_check(line):
        # A line counts as a null check if it contains any of the forms below.

        # OQS_EXIT_IF_NULLPTR(ptr, ...)
        if re.search(r'\bOQS_EXIT_IF_NULLPTR\(', line):
            return True

        # ptr == NULL  or  ptr != NULL
        if re.search(r'[!=]=\s*NULL\b', line):
            return True

        # !ptr ending a condition, e.g. "if (!ptr)", "if (!a || !b)", "if (!s->buf[i])"
        if re.search(r'!\s*[A-Za-z_]\w*(?:->\w+|\.\w+|\[[^\]]*\])*\s*(?:\)|\|\||&&)', line):
            return True

        return False

    def skippable(line):
        # returns True if a line can be safetly skipped
        return not line.strip() or line.strip().startswith(('//', '/*', '*'))

    # Get all relivant files to check
    c_h_files = []
    for root in ('src', 'tests'):
        for path, dirs, files in os.walk(root):
            if any(fnmatch.fnmatch(path, pattern) for pattern in MALLOC_CHECK_IGNORED_DIRS):
                dirs[:] = []  # don't descend into ignored directories
                continue
            c_h_files += [os.path.join(path, f) for f in files if f.endswith(('.c', '.h', '.fragment'))]

    okay = True
    for fn in c_h_files:
        with open(fn) as f:
            lines = f.read().splitlines()
        i = 0
        while i < len(lines):
            line = lines[i]
            # skip non-allocations, opted-out lines, and allocations checked on the same line
            if (skippable(line)
                or not is_allocation(line)
                or 'IGNORE malloc-check' in line or is_null_check(line)
            ):
                i += 1
                continue
            start, j = i, i
            while True:
                while j < len(lines) and ';' not in lines[j]:  # end of the statement
                    j += 1
                j += 1
                while j < len(lines) and skippable(lines[j]):
                    j += 1
                if j < len(lines) and is_allocation(lines[j]):  # batched allocation
                    continue
                break
            if j >= len(lines) or not is_null_check(lines[j]):
                okay = False
                print(f"Unchecked allocation in {fn}:{start + 1}:{line.strip()}")
            i = j

    assert okay, ("Every OQS_MEM_malloc/calloc/strdup/aligned_alloc call must be immediately "
                  "followed by a null check (e.g. OQS_EXIT_IF_NULLPTR, or an explicit NULL / !ptr "
                  "test). If an allocation is intentionally left unchecked, add the comment "
                  "'// IGNORE malloc-check' on that line.")

if __name__ == "__main__":
    test_memory_functions()
    test_malloc_null_check()
    import sys
    pytest.main(sys.argv)
