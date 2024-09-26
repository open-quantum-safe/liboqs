# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys
import json

fips_kem = ["ML-KEM-512", "ML-KEM-768", "ML-KEM-1024"]
fips_sig = ["ML-DSA-44", "ML-DSA-65", "ML-DSA-87"]

ml_kem_encdec = "ACVP_Vectors/ML-KEM-encapDecap-FIPS203/internalProjection.json"
ml_kem_kg     = "ACVP_Vectors/ML-KEM-keyGen-FIPS203/internalProjection.json"

ml_dsa_kg     = "ACVP_Vectors/ML-DSA-keyGen-FIPS204/internalProjection.json"
ml_dsa_sig    = "ACVP_Vectors/ML-DSA-sigGen-FIPS204/internalProjection.json"
ml_dsa_ver    = "ACVP_Vectors/ML-DSA-sigVer-FIPS204/internalProjection.json"

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_acvp_vec_kem_keygen(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if not(kem_name in fips_kem): pytest.skip("Not supported")

    with open(os.path.join('tests', ml_kem_kg), 'r') as fp:
        ml_kem_kg_acvp  = json.load(fp)

        variantFound = False
        for variant in ml_kem_kg_acvp["testGroups"]:
            if variant["parameterSet"] == kem_name:
                variantFound = True
                for testCase in variant["tests"]:
                    d = testCase["d"]
                    z = testCase["z"]
                    pk = testCase["ek"]
                    sk = testCase["dk"]

                    build_dir = helpers.get_current_build_dir_name()
                    helpers.run_subprocess(
                        [f'{build_dir}/tests/vectors_kem', kem_name, "keyGen", d+z, pk, sk]
                    )

        assert(variantFound == True)

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_acvp_vec_kem_encdec_aft(kem_name):

    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if not(kem_name in fips_kem): pytest.skip("Not supported")

    with open(os.path.join('tests', ml_kem_encdec), 'r') as fp:
        ml_kem_encdec_acvp  = json.load(fp)

        variantFound = False
        for variant in ml_kem_encdec_acvp["testGroups"]:
            if variant["parameterSet"] == kem_name and variant["testType"] == "AFT":
                variantFound = True
                for testCase in variant["tests"]:
                    #prompt
                    pk = testCase["ek"]
                    # TODO: need dk?
                    m = testCase["m"]
                    #expected results
                    k = testCase["k"]
                    c = testCase["c"]

                    build_dir = helpers.get_current_build_dir_name()
                    helpers.run_subprocess(
                        [f'{build_dir}/tests/vectors_kem', kem_name, "encDecAFT", m, pk, k, c]
                    )

        assert(variantFound == True)

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_acvp_vec_kem_encdec_val(kem_name):

    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if not(kem_name in fips_kem): pytest.skip("Not supported")

    with open(os.path.join('tests', ml_kem_encdec), 'r') as fp:
        ml_kem_encdec_acvp  = json.load(fp)

        variantFound = False
        for variant in ml_kem_encdec_acvp["testGroups"]:
            if variant["parameterSet"] == kem_name and variant["testType"] == "VAL":
                variantFound = True
                sk = variant["dk"]
                for testCase in variant["tests"]:
                    #prompt
                    c = testCase["c"]
                    #expected results
                    k = testCase["k"]

                    build_dir = helpers.get_current_build_dir_name()
                    helpers.run_subprocess(
                        [f'{build_dir}/tests/vectors_kem', kem_name, "encDecVAL", sk, k, c]
                    )

        assert(variantFound == True)


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
