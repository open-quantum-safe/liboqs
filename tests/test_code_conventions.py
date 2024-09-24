# SPDX-License-Identifier: MIT

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
    c_files = []
    for path, _, files in os.walk('src'):
        c_files += [os.path.join(path, f) for f in files if f.endswith('.c')]

    memory_functions = ['free', 'malloc', 'calloc', 'realloc', 'strdup']
    okay = True

    for fn in c_files:
        with open(fn) as f:
            content = f.read()
            lines = content.splitlines()
            for no, line in enumerate(lines, 1):
                # Skip comments
                if line.strip().startswith('//') or line.strip().startswith('/*'):
                    continue
                # Check if we're inside a multi-line comment
                if '/*' in content[:content.find(line)] and '*/' not in content[:content.find(line)]:
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

if __name__ == "__main__":
    test_memory_functions()
    import sys
    pytest.main(sys.argv)
