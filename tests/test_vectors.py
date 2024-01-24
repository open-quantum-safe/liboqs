# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_vectors():

    result = helpers.run_subprocess(
        ['tests/test_vectors.sh']
    )

    print("Test vectors mismatch.")
    print(result)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
