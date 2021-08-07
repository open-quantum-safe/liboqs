# SPDX-License-Identifier: MIT

import helpers
import os
import os.path
import pytest
import platform
import yaml

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_alg_info_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    # get the algorithm info from liboqs
    output = helpers.run_subprocess([helpers.path_to_executable('dump_alg_info')])
    alg_info = yaml.safe_load(output)['KEMs'][kem_name]
    assert(not(alg_info['isnull']))
    # find and load the datasheet
    datasheet_filename = helpers.run_subprocess(['grep', '-r', '-l', kem_name, 'docs/algorithms/kem']).splitlines()[0]
    datasheet_filename = datasheet_filename.replace('.md','.yml')
    with open(datasheet_filename, 'r', encoding='utf8') as datasheet_fh:
        datasheet = yaml.safe_load(datasheet_fh.read())
    # find the parameter set in the datasheet
    foundit = False
    for parameter_set in datasheet['parameter-sets']:
        if parameter_set['name'] == kem_name:
            foundit = True
            # check that the values match
            assert(alg_info['claimed-nist-level'] == parameter_set['claimed-nist-level'])
            assert(alg_info['claimed-security'] == parameter_set['claimed-security'])
            assert(alg_info['length-public-key'] == parameter_set['length-public-key'])
            assert(alg_info['length-ciphertext'] == parameter_set['length-ciphertext'])
            assert(alg_info['length-secret-key'] == parameter_set['length-secret-key'])
            assert(alg_info['length-shared-secret'] == parameter_set['length-shared-secret'])
            print("{:s} datasheet matches C algorithm info".format(kem_name))
            break
    assert(foundit)

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_alg_info_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    # get the algorithm info from liboqs
    output = helpers.run_subprocess([helpers.path_to_executable('dump_alg_info')])
    alg_info = yaml.safe_load(output)['SIGs'][sig_name]
    assert(not(alg_info['isnull']))
    # find and load the datasheet
    datasheet_filename = helpers.run_subprocess(['grep', '-r', '-l', sig_name, 'docs/algorithms/sig']).splitlines()[0]
    datasheet_filename = datasheet_filename.replace('.md','.yml')
    with open(datasheet_filename, 'r', encoding='utf8') as datasheet_fh:
        datasheet = yaml.safe_load(datasheet_fh.read())
    # find the parameter set in the datasheet
    foundit = False
    for parameter_set in datasheet['parameter-sets']:
        if parameter_set['name'] == sig_name:
            foundit = True
            # check that the values match
            assert(alg_info['claimed-nist-level'] == parameter_set['claimed-nist-level'])
            assert(alg_info['claimed-security'] == parameter_set['claimed-security'])
            assert(alg_info['length-public-key'] == parameter_set['length-public-key'])
            assert(alg_info['length-secret-key'] == parameter_set['length-secret-key'])
            assert(alg_info['length-signature'] == parameter_set['length-signature'])
            print("{:s} datasheet matches C algorithm info".format(sig_name))
            break
    assert(foundit)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

