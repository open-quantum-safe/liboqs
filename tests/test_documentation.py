# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_datasheet_kem(kem_name):
    helpers.run_subprocess(
        ['grep', '-r', kem_name, 'docs/algorithms']
    )

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_datasheet_sig(sig_name):
    helpers.run_subprocess(
        ['grep', '-r', sig_name, 'docs/algorithms']
    )

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_doxygen(build_tool):
    helpers.run_subprocess(
        [build_tool, 'gen_docs'],
        working_dir="build"
    )

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
