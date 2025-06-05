# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys
import json

import subprocess

fips_kem = ["ML-KEM-512", "ML-KEM-768", "ML-KEM-1024"]

ml_kem_strcmp = "Wycheproof_Vectors/mlkem_test/mlkem_test.json"
ml_kem_modOverflow = "Wycheproof_Vectors/mlkem_test/mlkem_test.json"

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_wpf_strcmp_vec(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if not(kem_name in fips_kem): pytest.skip("Not supported")

    with open(os.path.join('tests', ml_kem_strcmp), 'r', encoding='utf-8') as fp:
        ml_kem_kg_wpf  = json.load(fp)

        variantFound = False
        for variant in ml_kem_kg_wpf["testGroups"]:
            if variant["parameterSet"] == kem_name and variant["type"] == "MLKEMTest":
                variantFound = True
                for testCase in variant["tests"]:
                    seed = testCase["seed"]
                    ek = testCase["ek"]
                    c = testCase["c"]
                    k = testCase["K"]
                    
                    build_dir = helpers.get_current_build_dir_name()
                    helpers.run_subprocess(
                        [f'{build_dir}/tests/vectors_kem', kem_name, "strcmp", seed, ek, c, k]
                    )

        assert(variantFound == True)

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_wpf_modOverflow_vec(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if not(kem_name in fips_kem): pytest.skip("Not supported")

    with open(os.path.join('tests', ml_kem_modOverflow), 'r', encoding='utf-8') as fp:
        ml_kem_kg_wpf  = json.load(fp)

        variantFound = False
        for variant in ml_kem_kg_wpf["testGroups"]:
            if variant["parameterSet"] == kem_name and variant["type"] == "MLKEMEncapsTest":
                variantFound = True
                for testCase in variant["tests"]:
                    ek = testCase["ek"]

                    build_dir = helpers.get_current_build_dir_name()
                    helpers.run_subprocess(
                        [f'{build_dir}/tests/vectors_kem', kem_name, "modOverflow", ek]
                    )

        assert(variantFound == True)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)