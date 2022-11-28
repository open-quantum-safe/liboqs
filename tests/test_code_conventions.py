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

# Ensure "free" is not used unprotected in the main OQS code.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_free():
    c_files = []
    for path, _, files in os.walk('src'):
        c_files += [os.path.join(path,f) for f in files if f[-2:] == '.c']
    okay = True
    for fn in c_files:
        with open(fn) as f:
            # Find all lines that contain 'free(' but not '_free('
            for no, line in enumerate(f,1):
                if not re.match(r'^.*[^_]free\(.*$', line):
                    continue
                if 'IGNORE free-check' in line:
                    continue
                okay = False
                print("Suspicious `free` in {}:{}:{}".format(fn,no,line))
    assert okay, "'free' is used in some files.  These should be changed to 'OQS_MEM_secure_free' or 'OQS_MEM_insecure_free' as appropriate. If you are sure you want to use 'free' in a particular spot, add the comment '// IGNORE free-check' on the line where 'free' occurs."

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
