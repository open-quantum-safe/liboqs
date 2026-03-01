# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import sys
import json

import subprocess
import functools
import unittest.mock
from urllib.parse import urljoin
import requests

WYCHE_ROOT = "https://raw.githubusercontent.com/C2SP/wycheproof/main/testvectors_v1/"

# Maps supported ML-KEM algorithms to their respective public key (ek) and ciphertext (c) byte lengths.
ML_KEM_PARAMS = {
    "ML-KEM-512":  {"pk": 800,  "c": 768},
    "ML-KEM-768":  {"pk": 1184, "c": 1088},
    "ML-KEM-1024": {"pk": 1568, "c": 1568},
}

@pytest.fixture(autouse=True, scope="module")
def requests_get():
    with unittest.mock.patch("requests.get", wraps=requests.get) as mock_get:
        yield mock_get

@functools.lru_cache
def cached_requests_get(url: str):
    return requests.get(url)

def fetch_wycheproof_test_cases(kem_name, suffix, valid_types):
    """
    Generator that fetches Wycheproof vectors and yields individual test cases.
    Handles URL formatting, downloading, and filtering by test group type.
    """
    alg_id = kem_name.lower().replace("ml-kem-", "mlkem_")
    url = urljoin(WYCHE_ROOT, f"{alg_id}_{suffix}.json")
    
    resp = cached_requests_get(url)
    if resp.status_code != 200:
        return

    data = json.loads(resp.content)
    for group in data.get("testGroups", []):
        if group["parameterSet"] == kem_name and group["type"] in valid_types:
            yield from group["tests"]

def run_test_case(cmd, tc):
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc["result"] == "valid"
    cmd_type = cmd[2]

    # 1. Modulus Overflow edge case
    if cmd_type == "modOverflow":
        # vector_kem returns 0 only if it successfully blocked the unreduced key
        assert result.returncode == 0, (
            f"TC {tc['tcId']} FAILED: Accepted unreduced key.\nCmd: {' '.join(cmd)}"
        )
        return

    # 2. Structural Errors & Expected Rejections
    if not is_valid:
        # Invalid inputs must cause the vector_kem to return a non-zero exit code
        assert result.returncode != 0, (
            f"TC {tc['tcId']} FAILED: Expected rejection, but binary accepted it.\nCmd: {' '.join(cmd)}"
        )
        return

    # 3. Standard Valid Tests
    if cmd_type == "encDecVAL" and "K" not in tc:
        # Edge Case: Wycheproof's 'semi_expanded' file omits 'K' for valid decapsulation tests.
        # Since we padded 'K' with dummy zeroes for argv parsing, the vector_kem memcmp will 
        # intentionally fail (returncode 1). We verify success by checking if it completed decapsulation.
        assert "k: " in result.stdout, (
            f"TC {tc['tcId']} FAILED: Decapsulation failed early.\n"
            f"Stderr: {result.stderr}\nCmd: {' '.join(cmd)}"
        )
    else:
        # All other valid tests must exit cleanly
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
    test_cases = list(fetch_wycheproof_test_cases(kem_name, "keygen_seed_test", ["MLKEMKeyGen"]))
    assert test_cases, "No keygen test cases found."

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
    test_cases = list(fetch_wycheproof_test_cases(kem_name, "encaps_test", ["MLKEMEncapsTest"]))
    assert test_cases, "No encaps test cases found."

    pk_len = ML_KEM_PARAMS[kem_name]["pk"]
    c_len = ML_KEM_PARAMS[kem_name]["c"]

    for tc in test_cases:
        # Extract inputs, providing empty strings if they are missing
        m = tc.get("m", "")
        ek = tc.get("ek", "")
        expected_k = tc.get("K", "")
        expected_c = tc.get("c", "")

        is_invalid_pk = not expected_c or not expected_k or "ModulusOverflow" in tc.get("flags", [])
        
        # If the key is the correct length but mathematically invalid, use modOverflow.
        # Otherwise, pass it to standard encapsulation (short keys will fail the C strlen check).
        if is_invalid_pk and len(ek) == 2 * pk_len:
            cmd = [
                f"{build_dir}/tests/vectors_kem",
                kem_name, "modOverflow", ek
            ]
        else:
            # We provide dummy zeroes for missing fields so the vector_kem argv parser 
            # doesn't crash, allowing the C code to gracefully reject the bad data.
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
    test_cases_found = False
    c_len = ML_KEM_PARAMS[kem_name]["c"]
    
    # Wycheproof splits decaps tests into structural/functional (semi_expanded) and edge-cases (test)
    for suffix in ["test", "semi_expanded_decaps_test"]:
        for tc in fetch_wycheproof_test_cases(kem_name, suffix, ["MLKEMDecapsValidationTest", "MLKEMTest"]):
            test_cases_found = True
            
            # Pad missing fields for vector_kem argv stability
            expected_k = tc.get("K") or "00" * 32
            c = tc.get("c") or "00" * c_len

            # Seed-based tests (e.g. constant-time strcmp bug checks)
            if "seed" in tc and "ek" in tc:
                cmd = [
                    f"{build_dir}/tests/vectors_kem",
                    kem_name, "strcmp",
                    tc["seed"], tc["ek"], c, expected_k
                ]
            # Static key decapsulation tests
            elif "dk" in tc:
                cmd = [
                    f"{build_dir}/tests/vectors_kem",
                    kem_name, "encDecVAL",
                    tc["dk"], expected_k, c
                ]
            else:
                continue 

            run_test_case(cmd, tc)
            
    assert test_cases_found, "No decaps test cases found."

if __name__ == "__main__":
    pytest.main(sys.argv)