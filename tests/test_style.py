# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_style():

    result = helpers.run_subprocess(
        ['tests/test_style.sh']
    )
    assert 'Formatted' not in result

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

