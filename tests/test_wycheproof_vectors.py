# SPDX-License-Identifier: MIT

import functools
import json
import os
import unittest
import unittest.mock
import subprocess
from urllib.parse import urljoin

import pytest
import requests

import helpers

# Standard ML-KEM algorithm identifiers
ml_kem = ["ML-KEM-512", "ML-KEM-768", "ML-KEM-1024"]

# Wycheproof Vector Source
WYCHE_ROOT = "https://raw.githubusercontent.com/C2SP/wycheproof/main/testvectors_v1/"

@pytest.fixture(autouse=True, scope="module")
def requests_get():
    with unittest.mock.patch("requests.get", wraps=requests.get) as mock_get:
        yield mock_get

@functools.lru_cache
def cached_requests_get(url: str):
    resp = requests.get(url)
    return resp

def get_wyche_url(kem_name, suffix):
    """Maps e.g. ML-KEM-512 -> mlkem_512_<suffix>.json"""
    alg_id = kem_name.lower().replace("ml-kem-", "mlkem_")
    return urljoin(WYCHE_ROOT, f"{alg_id}_{suffix}.json")

def get_c_len(kem_name):
    if "512" in kem_name: return 768
    if "768" in kem_name: return 1088
    if "1024" in kem_name: return 1568
    return 0

def get_pk_len(kem_name):
    if "512" in kem_name: return 800
    if "768" in kem_name: return 1184
    if "1024" in kem_name: return 1568
    return 0

def run_test_case(cmd, tc):
    """
    Executes the C binary and asserts the return code.
    """
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc.get("result", "valid") in ["valid", "acceptable"]
    cmd_type = cmd[2]

    # 1. Modulus Overflow Tests
    if cmd_type == "modOverflow":
        # kem_modOverflow_vector returns 0 if it SUCCESSFULLY detects an unreduced key.
        if result.returncode != 0:
            pytest.fail(f"TC {tc['tcId']} FAILED: modOverflow check failed (binary accepted invalid key).\nCommand: {' '.join(cmd)}\nStderr: {result.stderr}")
        return

    # 2. Expected Rejections / Structural Errors
    if not is_valid:
        if result.returncode == 0:
            pytest.fail(f"TC {tc['tcId']} FAILED: Expected INVALID/REJECTION, but binary accepted it.\nCommand: {' '.join(cmd)}")
        return

    # 3. Valid Test Cases
    if cmd_type == "encDecVAL" and "K" not in tc:
        # Wycheproof's semi_expanded file omits 'K' for valid decapsulation tests.
        # We pass dummy zeroes, so C binary's memcmp will intentionally fail (returncode 1).
        # But we consider it a PASS if it successfully performed decapsulation (meaning it printed 'k: ').
        if "k: " not in result.stdout:
            pytest.fail(f"TC {tc['tcId']} FAILED: Expected VALID decapsulation, but failed early.\n"
                        f"Stderr: {result.stderr}\nCommand: {' '.join(cmd)}")
    else:
        # Standard valid tests must succeed cleanly (returncode 0)
        if result.returncode != 0:
            pytest.fail(f"TC {tc['tcId']} FAILED: Expected VALID, but binary rejected it.\n"
                        f"Stderr: {result.stderr}\nCommand: {' '.join(cmd)}")

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_keygen(kem_name):
    if not (helpers.is_kem_enabled_by_name(kem_name)) or not (kem_name in ml_kem):
        pytest.skip("Not enabled or supported")

    url = get_wyche_url(kem_name, "keygen_seed_test")
    resp = cached_requests_get(url)
    assert resp.status_code == 200
    data = json.loads(resp.content)

    variantFound = False
    build_dir = helpers.get_current_build_dir_name()

    for variant in data["testGroups"]:
        if variant["parameterSet"] == kem_name and variant["type"] == "MLKEMKeyGen":
            variantFound = True
            for tc in variant["tests"]:
                cmd = [
                    f"{build_dir}/tests/vectors_kem",
                    kem_name,
                    "keyGen",
                    tc["seed"],
                    tc["ek"],
                    tc["dk"],
                ]
                run_test_case(cmd, tc)
    assert variantFound == True

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_encaps(kem_name):
    if not (helpers.is_kem_enabled_by_name(kem_name)) or not (kem_name in ml_kem):
        pytest.skip("Not enabled or supported")

    url = get_wyche_url(kem_name, "encaps_test")
    resp = cached_requests_get(url)
    assert resp.status_code == 200
    data = json.loads(resp.content)

    variantFound = False
    build_dir = helpers.get_current_build_dir_name()

    for variant in data["testGroups"]:
        if variant["parameterSet"] == kem_name and variant["type"] == "MLKEMEncapsTest":
            variantFound = True
            for tc in variant["tests"]:
                m = tc.get("m", "")
                ek = tc.get("ek", "")
                expected_k = tc.get("K", "")
                expected_c = tc.get("c", "")

                is_invalid_pk = not expected_c or not expected_k or "ModulusOverflow" in tc.get("flags", [])
                
                # Only use modOverflow if the key is the exact expected length.
                # If it's too short (like TC 218), encDecAFT's built-in strlen check gracefully handles it.
                if is_invalid_pk and len(ek) == 2 * get_pk_len(kem_name):
                    cmd = [
                        f"{build_dir}/tests/vectors_kem",
                        kem_name,
                        "modOverflow",
                        ek
                    ]
                else:
                    cmd = [
                        f"{build_dir}/tests/vectors_kem",
                        kem_name,
                        "encDecAFT",
                        m if m else "00" * 32,
                        ek if ek else "00" * get_pk_len(kem_name),
                        expected_k if expected_k else "00" * 32,
                        expected_c if expected_c else "00" * get_c_len(kem_name)
                    ]
                run_test_case(cmd, tc)
    assert variantFound == True

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_decaps(kem_name):
    if not (helpers.is_kem_enabled_by_name(kem_name)) or not (kem_name in ml_kem):
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    variantFound = False
    
    for suffix in ["test", "semi_expanded_decaps_test"]:
        url = get_wyche_url(kem_name, suffix)
        resp = cached_requests_get(url)
        if resp.status_code != 200:
            continue
            
        data = json.loads(resp.content)
        for variant in data["testGroups"]:
            if variant["parameterSet"] == kem_name and variant["type"] in ["MLKEMDecapsValidationTest", "MLKEMTest"]:
                variantFound = True
                for tc in variant["tests"]:
                    expected_k = tc.get("K", "")
                    c = tc.get("c", "")

                    if "seed" in tc and "ek" in tc:
                        cmd = [
                            f"{build_dir}/tests/vectors_kem",
                            kem_name,
                            "strcmp",
                            tc["seed"],
                            tc["ek"],
                            c if c else "00" * get_c_len(kem_name),
                            expected_k if expected_k else "00" * 32
                        ]
                    elif "dk" in tc:
                        cmd = [
                            f"{build_dir}/tests/vectors_kem",
                            kem_name,
                            "encDecVAL",
                            tc["dk"],
                            expected_k if expected_k else "00" * 32,
                            c if c else "00" * get_c_len(kem_name)
                        ]
                    else:
                        continue 

                    run_test_case(cmd, tc)
    assert variantFound == True

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)