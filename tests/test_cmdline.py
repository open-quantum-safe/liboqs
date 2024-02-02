# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import sys

@helpers.filtered_test
@pytest.mark.parametrize('program', ['example_kem', 'example_sig'])
def test_examples(program):
    helpers.run_subprocess(
        [helpers.path_to_executable(program)],
    )

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    helpers.run_subprocess(
        [helpers.path_to_executable('test_kem'), kem_name],
    )

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    helpers.run_subprocess(
        [helpers.path_to_executable('test_sig'), sig_name],
    )

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

