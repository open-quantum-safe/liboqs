# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import sys

@helpers.filtered_test
@pytest.mark.parametrize('program', ['example_kem', 'example_sig', 'example_sig_stfl'])
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

@helpers.filtered_test
@pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
def test_sig_stfl(sig_stfl_name):
    # Define the list of LMS varients to allow
    names_to_allow = ["LMS_SHA256_H5_W1", "LMS_SHA256_H5_W2", 
                        "LMS_SHA256_H5_W4",
                        "LMS_SHA256_H5_W8",
                        "LMS_SHA256_H10_W1",
                        "LMS_SHA256_H10_W2",
                        "LMS_SHA256_H10_W4",
                        "LMS_SHA256_H10_W8",
                        "LMS_SHA256_H15_W1",
                        "LMS_SHA256_H15_W2",
                        "LMS_SHA256_H15_W4",
                        "LMS_SHA256_H5_W8_H5_W8",
                        "LMS_SHA256_H10_W4_H5_W8",]
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
    # Test with KATs apply for XMSS
    if sig_stfl_name.startswith("XMSS"):
        katfile = helpers.get_katfile("sig_stfl", sig_stfl_name)
        if not katfile: pytest.skip("KATs file is missing")
        helpers.run_subprocess(
            [helpers.path_to_executable('test_sig_stfl'), sig_stfl_name, katfile],
        )
    else:
        if sig_stfl_name not in names_to_allow:
            pytest.skip('Skip slow stateful hash-based signature test.')
        helpers.run_subprocess(
            [helpers.path_to_executable('test_sig_stfl'), sig_stfl_name],
            )

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

