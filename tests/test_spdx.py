# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys

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

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

