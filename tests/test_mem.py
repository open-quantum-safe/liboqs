# SPDX-License-Identifier: MIT

import helpers
import pytest
from pathlib import Path

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_mem_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip('Not enabled')

    Path(helpers.get_current_build_dir_name()+'/mem-benchmark').mkdir(parents=True, exist_ok=True)

    for i in range(3):
       helpers.run_subprocess([helpers.path_to_executable('test_kem_mem'), kem_name, str(i)])

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_mem_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip('Not enabled')

    Path(helpers.get_current_build_dir_name()+'/mem-benchmark').mkdir(parents=True, exist_ok=True)

    for i in range(3):
       helpers.run_subprocess([helpers.path_to_executable('test_sig_mem'), sig_name, str(i)])

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

