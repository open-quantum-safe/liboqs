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

MlDsaParam = namedtuple("MlDsaParam", ["pk", "sk"])
# Add lengths for ML-DSA so we can generate dummy strings for key expansion
ML_DSA_PARAMS = {
    "ML-DSA-44": MlDsaParam(1312, 2560),
    "ML-DSA-65": MlDsaParam(1952, 4032),
    "ML-DSA-87": MlDsaParam(2592, 4896),
}

def fetch_wycheproof_kem_test_cases(kem_name, suffix, valid_types):
    """
    Fetches Wycheproof vectors and returns a list of individual test cases.
    Handles URL formatting, downloading, and filtering by test group type for ML-KEM.
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
    Handles URL formatting, downloading, and filtering by test group type for ML-DSA.
    """
    alg_id = sig_name.lower().replace("ml-dsa-", "mldsa_")
    url = urljoin(WYCHE_ROOT, f"{alg_id}_{suffix}.json")
    
    resp = helpers.cached_requests_get(url)
    resp.raise_for_status()

    data = json.loads(resp.content)
    test_cases = []
    
    for group in data.get("testGroups", []):
        if group["type"] in valid_types:
            # We return (group, tc) because ML-DSA requires group-level public/private keys
            for tc in group["tests"]:
                test_cases.append((group, tc))

    if not test_cases:
        raise ValueError(f"Malformed test data or no matching test groups for {sig_name} in {url}")
            
    return test_cases

def expand_private_key_from_seed(build_dir, sig_name, seed):
    """
    Wycheproof 'seed' test vectors provide a 32-byte seed instead of the fully 
    expanded private key required by liboqs. This function dynamically expands it 
    by calling the vector_sig keyGen command and parsing the stdout.
    """
    if sig_name not in ML_DSA_PARAMS:
        return None
        
    pk_len = ML_DSA_PARAMS[sig_name].pk
    sk_len = ML_DSA_PARAMS[sig_name].sk
    
    cmd = [
        f"{build_dir}/tests/vectors_sig",
        sig_name, "keyGen",
        seed,
        "00" * pk_len,
        "00" * sk_len
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    for line in result.stdout.splitlines():
        if line.startswith("sk: "):
            return line.split("sk: ")[1].strip()
            
    return None

def run_kem_test_case(cmd, tc):
    """
    Executes vector_kem and verifies the exit code.
    The C test harness internally checks values via memcmp() and returns 0 for success.
    """
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc.get("result") == "valid"
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

def run_sig_test_case(cmd, tc):
    """
    Executes vectors_sig and verifies the exit code against Wycheproof expectations.
    """
    result = subprocess.run(cmd, capture_output=True, text=True)
    is_valid = tc.get("result") == "valid"
    if "sigVer" in cmd[2]:
        # Return code 0 means exact match with expected behavior.
        if result.returncode != 0:
            # Scenario 1 (PASS): Early Rejection. 
            # Invalid input (e.g., bad length) caught before crypto API.
            if not is_valid and "ERROR: OQS_SIG_verify" not in result.stderr and "failed!" not in result.stderr:
                return 
            # Scenario 2 (FAIL): Cryptographic False Accept. 
            # API successfully verified a forged/invalid signature.
            elif not is_valid and "failed!" in result.stderr:
                pytest.fail(f"TC {tc['tcId']} FAILED: Binary accepted an invalid signature.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
            # Scenario 3 (FAIL): Unexpected Failure.
            # Valid signature failed verification, or binary crashed.
            else:
                pytest.fail(f"TC {tc['tcId']} FAILED: Verification assertion mismatch.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
        # Scenario 4 (PASS): Expected behavior (Valid accepted OR Invalid cryptographically rejected).
    else:
        if result.returncode != 0:
            # Scenario 1 (PASS): Expected Failure.
            # Invalid inputs (e.g., bad key) naturally failed to generate a signature.
            if not is_valid:
                return  
            # Scenario 2 (FAIL): Mismatch or Crash.
            # Valid inputs resulted in bad math; signature didn't match JSON.
            else:
                pytest.fail(f"TC {tc['tcId']} FAILED: Signature generation failed or mismatched.\n"
                            f"Stderr: {result.stderr}\nStdout: {result.stdout}\nCmd: {' '.join(cmd)}")
        else: 
            # Scenario 3 (FAIL): False Match.
            # Invalid test inputs somehow perfectly generated the dummy signature.
            if not is_valid:
                pytest.fail(f"TC {tc['tcId']} FAILED: Generated signature unexpectedly matched invalid test.\n"
                            f"Cmd: {' '.join(cmd)}")
            
@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_keygen(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    test_cases = fetch_wycheproof_kem_test_cases(kem_name, "keygen_seed_test", ["MLKEMKeyGen"])

    for tc in test_cases:
        cmd = [
            f"{build_dir}/tests/vectors_kem",
            kem_name, "keyGen",
            tc["seed"], tc["ek"], tc["dk"]
        ]
        run_kem_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_encaps(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    test_cases = fetch_wycheproof_kem_test_cases(kem_name, "encaps_test", ["MLKEMEncapsTest"])

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
        run_kem_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_wycheproof_vec_kem_decaps(kem_name):
    if not helpers.is_kem_enabled_by_name(kem_name) or kem_name not in ML_KEM_PARAMS:
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    c_len = ML_KEM_PARAMS[kem_name].ct
    
    for suffix in ["test", "semi_expanded_decaps_test"]:
        test_cases = fetch_wycheproof_kem_test_cases(kem_name, suffix, ["MLKEMDecapsValidationTest", "MLKEMTest"])
        
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

            run_kem_test_case(cmd, tc)

@helpers.filtered_test
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_wycheproof_vec_sig_verify(sig_name):
    if not helpers.is_sig_enabled_by_name(sig_name) or not sig_name.startswith("ML-DSA"):
        pytest.skip("Not enabled or supported")

    build_dir = helpers.get_current_build_dir_name()
    test_cases = fetch_wycheproof_sig_test_cases(sig_name, "verify_test", ["MlDsaVerify"])
    
    if not test_cases:
        pytest.skip("No verify test cases found.")

    for group, tc in test_cases:
        pk = tc.get("publicKey", group.get("publicKey", ""))
        
        msg = tc.get("msg", "")
        sig = tc.get("sig", "")
        ctx = tc.get("ctx", "")
        mu = tc.get("mu", "")
        
        is_valid = tc.get("result") == "valid"
        testPassed = "1" if is_valid else "0"
        flags = tc.get("flags", [])

        if "Internal" in flags or (mu and not msg):
            cmd = [
                f"{build_dir}/tests/vectors_sig",
                sig_name, "sigVer_int",
                pk, mu, sig, testPassed, "1"
            ]
        else:
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

    for suffix in ["sign_noseed_test", "sign_seed_test"]:
        test_cases = fetch_wycheproof_sig_test_cases(sig_name, suffix, ["MlDsaSign"])
        if not test_cases:
            pytest.skip("No verify test cases found.")

        for group, tc in test_cases:
            # 1. Try to fetch the fully expanded private key directly (standard for 'noseed' files)
            sk = tc.get("privateKey") or group.get("privateKey")
            
            # 2. If missing, it's a 'seed' file. Extract the 32-byte seed and dynamically expand it.
            if not sk:
                seed = tc.get("privateSeed") or group.get("privateSeed")
                if seed:
                    sk = expand_private_key_from_seed(build_dir, sig_name, seed)
            # Passing None/empty to the subprocess would crash Python or fail the vector_sig arg parser.
            # Skip and move to next
            if not sk:
                continue
                
            msg = tc.get("msg", "")
            sig = tc.get("sig", "")
            ctx = tc.get("ctx", "")
            mu = tc.get("mu", "")
            flags = tc.get("flags", [])
            
            # Wycheproof 'seed' files provide randomized 'rnd'. 'noseed' files do not, so fallback to zeros.
            rnd = tc.get("rnd", "00" * 32)

            # 4. Route to the Internal API (pre-hashed) or External API (standard)
            if "Internal" in flags or (mu and not msg):
                cmd = [
                    f"{build_dir}/tests/vectors_sig",
                    sig_name, "sigGen_int",
                    sk, mu, sig, rnd, "1"
                ]
            else:
                cmd = [
                    f"{build_dir}/tests/vectors_sig",
                    sig_name, "sigGen_ext",
                    sk, msg, sig, ctx, rnd
                ]
                
            run_sig_test_case(cmd, tc)

if __name__ == "__main__":
    pytest.main(sys.argv)

