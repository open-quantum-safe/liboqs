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

def fetch_wycheproof_sig_test_cases(sig_name, suffix, valid_types):
    """
    Fetches Wycheproof vectors and returns a list of (group, test_case) tuples.
    Handles URL formatting, local file fallback, and filtering by test group type for ML-DSA.
    """
    base_alg = sig_name.replace("-extmu", "")
    alg_id = base_alg.lower().replace("ml-dsa-", "mldsa_")
    file_name = f"{alg_id}_{suffix}.json"
    
    if os.path.exists(file_name):
        with open(file_name, "r") as f:
            data = json.load(f)
    else:
        url = urljoin(WYCHE_ROOT, file_name)
        resp = helpers.cached_requests_get(url)
        
        if resp.status_code == 404:
            raise FileNotFoundError(
                f"The file '{file_name}' was not found locally and is not available upstream at {url}."
            )
        resp.raise_for_status()
        data = json.loads(resp.content)

    test_cases = []
    for group in data.get("testGroups", []):
        if group["type"] in valid_types:
            for tc in group["tests"]:
                test_cases.append((group, tc))
                
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
        assert result.returncode == 0, (
            f"TC {tc['tcId']} FAILED: Accepted unreduced key.\nCmd: {' '.join(cmd)}"
        )
    elif not is_valid:
        # 2. Structural Errors & Expected Rejections
        assert result.returncode != 0, (
            f"TC {tc['tcId']} FAILED: Expected rejection, but binary accepted it.\nCmd: {' '.join(cmd)}"
        )
    elif cmd_type == "encDecVAL" and "K" not in tc:
        # 3. Standard Valid Tests (Edge Case)
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

def run_sig_test_case(cmd, tc):
    """
    Executes vectors_sig and verifies the exit code.
    """
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc.get("result", "valid") in ["valid", "acceptable"]
    
    if "sigVer" in cmd[2]:
        if result.returncode != 0:
            if not is_valid and "ERROR: OQS_SIG_verify" not in result.stderr and "failed!" not in result.stderr:
                return 
            elif not is_valid and "failed!" in result.stderr:
                pytest.fail(f"TC {tc['tcId']} FAILED: Binary accepted an invalid signature.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
            else:
                pytest.fail(f"TC {tc['tcId']} FAILED: Verification assertion mismatch.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
    else:
        if result.returncode != 0:
            if not is_valid:
                return # Passed: Generation failed or mismatched as correctly anticipated by Wycheproof
            else:
                pytest.fail(f"TC {tc['tcId']} FAILED: Signature generation failed or mismatched.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
        else:
            if not is_valid:
                pytest.fail(f"TC {tc['tcId']} FAILED: Generated signature unexpectedly matched invalid test.\n"
                            f"Cmd: {' '.join(cmd)}")

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

@helpers.filtered_test
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_wycheproof_vec_sig_verify(sig_name):
    if not helpers.is_sig_enabled_by_name(sig_name) or not sig_name.startswith("ML-DSA"):
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    is_extmu = sig_name.endswith("-extmu")

    test_cases = fetch_wycheproof_sig_test_cases(sig_name, "verify_test", ["MlDsaVerify"])
    if not test_cases:
        pytest.skip("No verify test cases found.")

    for group, tc in test_cases:
        pk = group.get("publicKey", "")
        msg = tc.get("msg", "")
        sig = tc.get("sig", "")
        ctx = tc.get("ctx", "")
        mu = tc.get("mu", "")
        
        is_valid = tc.get("result", "valid") in ["valid", "acceptable"]
        testPassed = "1" if is_valid else "0"
        flags = tc.get("flags", [])

        if is_extmu:
            if not mu:
                continue
            cmd = [
                f"{build_dir}/tests/vectors_sig",
                sig_name, "sigVer_extmu",
                pk, mu, sig, testPassed
            ]
        else:
            if "Internal" in flags or (mu and not msg):
                continue
            cmd = [
                f"{build_dir}/tests/vectors_sig",
                sig_name, "sigVer_ext",
                pk, msg, sig, ctx, testPassed
            ]
        
        run_sig_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_wycheproof_vec_sig_sign(sig_name):
    if not helpers.is_sig_enabled_by_name(sig_name) or not sig_name.startswith("ML-DSA"):
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    is_extmu = sig_name.endswith("-extmu")

    test_cases = fetch_wycheproof_sig_test_cases(sig_name, "sign_noseed_test", ["MlDsaSign"])
    if not test_cases:
        pytest.skip("No sign test cases found.")

    for group, tc in test_cases:
        sk = group.get("privateKey", "")
        if not sk:
            continue
            
        msg = tc.get("msg", "")
        sig = tc.get("sig", "")
        ctx = tc.get("ctx", "")
        mu = tc.get("mu", "")
        flags = tc.get("flags", [])

        if is_extmu:
            if not mu:
                continue
            cmd = [
                f"{build_dir}/tests/vectors_sig",
                sig_name, "sigGen_extmu",
                sk, mu, sig, "00" * 32
            ]
        else:
            if "Internal" in flags or (mu and not msg):
                continue
            cmd = [
                f"{build_dir}/tests/vectors_sig",
                sig_name, "sigGen_ext",
                sk, msg, sig, ctx, "00" * 32
            ]
            
        run_sig_test_case(cmd, tc)

if __name__ == "__main__":
    pytest.main(sys.argv)