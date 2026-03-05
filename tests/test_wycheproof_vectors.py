# SPDX-License-Identifier: MIT

import json
import os
import subprocess
import sys
from urllib.parse import urljoin
from collections import namedtuple

import pytest
import helpers
from helpers import requests_get, cached_requests_get

WYCHE_ROOT = "https://raw.githubusercontent.com/C2SP/wycheproof/main/testvectors_v1/"

MlKemParam = namedtuple("MlKemParam", ["pk", "ct"])

# Maps supported ML-KEM algorithms to their respective public key (ek) and ciphertext (c) byte lengths.
ML_KEM_PARAMS = {
    "ML-KEM-512":  MlKemParam(800, 768),
    "ML-KEM-768":  MlKemParam(1184, 1088),
    "ML-KEM-1024": MlKemParam(1568, 1568),
}

def fetch_wycheproof_test_cases(kem_name, suffix, valid_types):
    """
    Fetches Wycheproof vectors and returns a list of individual test cases.
    Handles URL formatting, downloading, and filtering by test group type.
    """
    alg_id = kem_name.lower().replace("ml-kem-", "mlkem_")
    url = urljoin(WYCHE_ROOT, f"{alg_id}_{suffix}.json")
    
    resp = helpers.cached_requests_get(url)
    resp.raise_for_status()

    data = json.loads(resp.content)
    test_cases = []
    
    for group in data.get("testGroups", []):
        if group["parameterSet"] == kem_name and group["type"] in valid_types:
            test_cases.extend(group["tests"])
            
    if not test_cases:
        raise ValueError(f"Malformed test data or no matching test groups for {kem_name} in {url}")
        
    return test_cases

def run_test_case(cmd, tc):
    """
    Executes vector_kem and verifies the exit code.
    The C test harness internally checks values via memcmp() and returns 0 for success.
    """
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc.get("result", "valid") in ["valid", "acceptable"]
    cmd_type = cmd[2]

    if cmd_type == "modOverflow":
        # 1. Modulus Overflow edge case
        # vector_kem returns 0 only if it successfully blocked the unreduced key
        assert result.returncode == 0, (
            f"TC {tc['tcId']} FAILED: Accepted unreduced key.\nCmd: {' '.join(cmd)}"
        )
    elif not is_valid:
        # 2. Structural Errors & Expected Rejections
        # Invalid inputs must cause vector_kem to return a non-zero exit code
        assert result.returncode != 0, (
            f"TC {tc['tcId']} FAILED: Expected rejection, but binary accepted it.\nCmd: {' '.join(cmd)}"
        )
    elif cmd_type == "encDecVAL" and "K" not in tc:
        # 3. Standard Valid Tests (Edge Case)
        # Wycheproof's 'semi_expanded' file omits 'K' for valid decapsulation tests.
        # Since we padded 'K' with dummy zeroes for argv parsing, vector_kem memcmp will 
        # intentionally fail (returncode 1). We verify success by checking if it completed decapsulation.
        assert "k: " in result.stdout, (
            f"TC {tc['tcId']} FAILED: Decapsulation failed early.\n"
            f"Stderr: {result.stderr}\nCmd: {' '.join(cmd)}"
        )
    else:
        # 4. All other valid tests must exit cleanly
        assert result.returncode == 0, (
            f"TC {tc['tcId']} FAILED: Rejected valid input.\n"
            f"Stderr: {result.stderr}\nCmd: {' '.join(cmd)}"
        )

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_keygen(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    test_cases = fetch_wycheproof_test_cases(kem_name, "keygen_seed_test", ["MLKEMKeyGen"])

    for tc in test_cases:
        cmd = [
            f"{build_dir}/tests/vectors_kem",
            kem_name, "keyGen",
            tc["seed"], tc["ek"], tc["dk"]
        ]
        run_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_encaps(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    test_cases = fetch_wycheproof_test_cases(kem_name, "encaps_test", ["MLKEMEncapsTest"])

    pk_len = ML_KEM_PARAMS[kem_name].pk
    c_len = ML_KEM_PARAMS[kem_name].ct

    for tc in test_cases:
        m = tc.get("m", "")
        ek = tc.get("ek", "")
        expected_k = tc.get("K", "")
        expected_c = tc.get("c", "")

        is_invalid_pk = not expected_c or not expected_k or "ModulusOverflow" in tc.get("flags", [])
        
        if is_invalid_pk and len(ek) == 2 * pk_len:
            cmd = [
                f"{build_dir}/tests/vectors_kem",
                kem_name, "modOverflow", ek
            ]
        else:
            cmd = [
                f"{build_dir}/tests/vectors_kem",
                kem_name, "encDecAFT",
                m or "00" * 32,
                ek or "00" * pk_len,
                expected_k or "00" * 32,
                expected_c or "00" * c_len
            ]
        run_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_decaps(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    c_len = ML_KEM_PARAMS[kem_name].ct
    
    for suffix in ["test", "semi_expanded_decaps_test"]:
        test_cases = fetch_wycheproof_test_cases(kem_name, suffix, ["MLKEMDecapsValidationTest", "MLKEMTest"])
        
        for tc in test_cases:
            expected_k = tc.get("K") or "00" * 32
            c = tc.get("c") or "00" * c_len

            if "seed" in tc and "ek" in tc:
                cmd = [
                    f"{build_dir}/tests/vectors_kem",
                    kem_name, "strcmp",
                    tc["seed"], tc["ek"], c, expected_k
                ]
            elif "dk" in tc:
                cmd = [
                    f"{build_dir}/tests/vectors_kem",
                    kem_name, "encDecVAL",
                    tc["dk"], expected_k, c
                ]
            else:
                continue 

            run_test_case(cmd, tc)

if __name__ == "__main__":
    pytest.main(sys.argv)
