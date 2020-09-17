# SPDX-License-Identifier: MIT

import helpers
import pytest
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
    files = helpers.run_subprocess(['find', 'src', '-name', '*.c'])
    files = helpers.run_subprocess(['grep', '-v', 'picnic/external'], input=files.encode('utf8'))
    lines = helpers.run_subprocess(['xargs', 'grep', '[^_]free('], input=files.encode('utf8'), ignore_returncode=True)
    lines = lines.split("\n")
    okay = True
    for line in lines:
        if line == "": continue
        if not('IGNORE free-check' in line):
            okay = False
            print("Suspicious `free` in " + line)
    assert okay, "'free' is used in some files.  These should be changed to 'OQS_MEM_secure_free' or 'OQS_MEM_insecure_free' as appropriate. If you are sure you want to use 'free' in a particular spot, add the comment '// IGNORE free-check' on the line where 'free' occurs."

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
