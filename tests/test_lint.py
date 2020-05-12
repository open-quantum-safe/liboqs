# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys

###
# Checks that "free" is not used unprotected in the main OQS code.
###
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_free():
    files = helpers.run_subprocess(['find', 'src', '-name', '*.c'])
    files = helpers.run_subprocess(['grep', '-v', 'picnic/external'], input=files.encode('utf8'))
    lines = helpers.run_subprocess(['xargs', 'grep', '[^_]free('], input=files.encode('utf8'))
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
