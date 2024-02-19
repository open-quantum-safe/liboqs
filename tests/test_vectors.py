# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_vectors_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled' + kem_name)
    result = helpers.run_subprocess(
        ['tests/test_vectors.sh', kem_name],
    )
    if kem_name + " not supported" in result: pytest.skip("Not supported")

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_vectors_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    result = helpers.run_subprocess(
        ['tests/test_vectors.sh', sig_name],
    )
    if sig_name + " not supported" in result: pytest.skip("Not supported")

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
