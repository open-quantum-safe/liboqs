# SPDX-License-Identifier: MIT

import helpers
import os
import os.path
import pytest
import platform
from hashlib import sha256

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    kats = helpers.get_kats("kem")
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    for i in range(3):
       helpers.run_subprocess([helpers.path_to_executable('test_kem_mem'), kem_name, str(i)])

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    for i in range(3):
       helpers.run_subprocess([helpers.path_to_executable('test_sig_mem'), sig_name, str(i)])

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

