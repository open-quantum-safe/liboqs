# SPDX-License-Identifier: MIT

import functools
import json
import sys
from urllib.parse import urljoin
import unittest
import unittest.mock

import pytest
import requests

import helpers
import hashlib

ml_kem = ["ML-KEM-512", "ML-KEM-768", "ML-KEM-1024"]
ml_sig = ["ML-DSA-44", "ML-DSA-65", "ML-DSA-87"]
slh_sig = []

# TODO: this could be a list comprehension
# Add all enabled slhdsa algs to slh_sig
for sig in helpers.available_sigs_by_name():
    if helpers.is_sig_enabled_by_name(sig) and sig.startswith("SLH_DSA"):
        slh_sig.append(sig)

# NOTE: these are not made into fixture because each is used once or twice, so
# the (probably non-existent) performance gain is not worth the loss of
# debuggability
URLROOT = "https://raw.githubusercontent.com/usnistgov/ACVP-Server/refs/tags/v1.1.0.41/gen-val/json-files/"
ml_kem_encdec = urljoin(URLROOT, "ML-KEM-encapDecap-FIPS203/internalProjection.json")
ml_kem_kg = urljoin(URLROOT, "ML-KEM-keyGen-FIPS203/internalProjection.json")

ml_dsa_kg = urljoin(URLROOT, "ML-DSA-keyGen-FIPS204/internalProjection.json")
ml_dsa_sig = urljoin(URLROOT, "ML-DSA-sigGen-FIPS204/internalProjection.json")
ml_dsa_ver = urljoin(URLROOT, "ML-DSA-sigVer-FIPS204/internalProjection.json")

slh_dsa_kg = urljoin(URLROOT, "SLH-DSA-keyGen-FIPS205/internalProjection.json")
slh_dsa_sig = urljoin(URLROOT, "SLH-DSA-sigGen-FIPS205/internalProjection.json")
slh_dsa_ver = urljoin(URLROOT, "SLH-DSA-sigVer-FIPS205/internalProjection.json")

def calc_hash(msg, algo):
    msg_bytes = bytes.fromhex(msg)

    if algo == "SHA2-224":
        return hashlib.sha224(msg_bytes).hexdigest()
    elif algo == "SHA2-256":
        return hashlib.sha256(msg_bytes).hexdigest()
    elif algo == "SHA2-384":
        return hashlib.sha384(msg_bytes).hexdigest()
    elif algo == "SHA2-512":
        return hashlib.sha512(msg_bytes).hexdigest()
    elif algo == "SHA2-512/224":
        return hashlib.new("sha512_224", msg_bytes).hexdigest()
    elif algo == "SHA2-512/256":
        return hashlib.new("sha512_256", msg_bytes).hexdigest()
    elif algo == "SHA3-224":
        return hashlib.sha3_224(msg_bytes).hexdigest()
    elif algo == "SHA3-256":
        return hashlib.sha3_256(msg_bytes).hexdigest()
    elif algo == "SHA3-384":
        return hashlib.sha3_384(msg_bytes).hexdigest()
    elif algo == "SHA3-512":
        return hashlib.sha3_512(msg_bytes).hexdigest()
    elif algo == "SHAKE-128":
        return hashlib.shake_128(msg_bytes).hexdigest(32)
    elif algo == "SHAKE-256":
        return hashlib.shake_256(msg_bytes).hexdigest(64)
    else:
        raise ValueError(f"Unsupported hash algorithm: {algo}")

@pytest.fixture(autouse=True, scope="module")
def requests_get():
    with unittest.mock.patch("requests.get", wraps=requests.get) as mock_get:
        yield mock_get
        print(f"mock_get is called {mock_get.call_count} times")


@functools.lru_cache
def cached_requests_get(url: str):
    resp = requests.get(url)
    return resp


@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_acvp_vec_kem_keygen(kem_name):
    # TODO: this logic feels backwards. It will instantiate kem_name with all
    # possible names, then filter out the overwhelming majority of them that are
    # "not ML-KEM". This creates a lot of skipped tests that, despite not having
    # much performance penalty, pollute the pytest output.
    if not (helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip("Not enabled")
    if not (kem_name in ml_kem):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_kem_kg)
    assert resp.status_code == 200
    ml_kem_kg_acvp = json.loads(resp.content)

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
                    [
                        f"{build_dir}/tests/vectors_kem",
                        kem_name,
                        "keyGen",
                        d + z,
                        pk,
                        sk,
                    ]
                )

    # TODO: assert xxx == True is not Pythonic
    assert variantFound == True


@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_acvp_vec_kem_encdec_aft(kem_name):

    if not (helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip("Not enabled")
    if not (kem_name in ml_kem):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_kem_encdec)
    assert resp.status_code == 200
    ml_kem_encdec_acvp = json.loads(resp.content)

    variantFound = False
    for variant in ml_kem_encdec_acvp["testGroups"]:
        if (
            variant["parameterSet"] == kem_name
            and variant["function"] == "encapsulation"
        ):
            variantFound = True
            for testCase in variant["tests"]:
                # prompt
                pk = testCase["ek"]
                # TODO: need dk?
                m = testCase["m"]
                # expected results
                k = testCase["k"]
                c = testCase["c"]

                build_dir = helpers.get_current_build_dir_name()
                helpers.run_subprocess(
                    [
                        f"{build_dir}/tests/vectors_kem",
                        kem_name,
                        "encDecAFT",
                        m,
                        pk,
                        k,
                        c,
                    ]
                )

    assert variantFound == True


@helpers.filtered_test
@pytest.mark.parametrize("kem_name", helpers.available_kems_by_name())
def test_acvp_vec_kem_encdec_val(kem_name):

    if not (helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip("Not enabled")
    if not (kem_name in ml_kem):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_kem_encdec)
    assert resp.status_code == 200
    ml_kem_encdec_acvp = json.loads(resp.content)

    variantFound = False
    for variant in ml_kem_encdec_acvp["testGroups"]:
        if (
            variant["parameterSet"] == kem_name
            and variant["function"] == "decapsulation"
        ):
            variantFound = True
            for testCase in variant["tests"]:
                sk = testCase["dk"]
                # prompt
                c = testCase["c"]
                # expected results
                k = testCase["k"]

                build_dir = helpers.get_current_build_dir_name()
                helpers.run_subprocess(
                    [
                        f"{build_dir}/tests/vectors_kem",
                        kem_name,
                        "encDecVAL",
                        sk,
                        k,
                        c,
                    ]
                )

    assert variantFound == True


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_ml_dsa_sig_keygen(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in ml_sig):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_dsa_kg)
    assert resp.status_code == 200
    ml_sig_kg_acvp = json.loads(resp.content)

    variantFound = False
    for variant in ml_sig_kg_acvp["testGroups"]:
        if variant["parameterSet"] == sig_name:
            variantFound = True
            for testCase in variant["tests"]:
                seed = testCase["seed"]
                pk = testCase["pk"]
                sk = testCase["sk"]

                build_dir = helpers.get_current_build_dir_name()
                helpers.run_subprocess(
                    [
                        f"{build_dir}/tests/vectors_sig",
                        sig_name,
                        "keyGen",
                        seed,
                        pk,
                        sk,
                    ]
                )

    assert variantFound == True


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_ml_dsa_sig_gen(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in ml_sig):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_dsa_sig)
    assert resp.status_code == 200
    ml_sig_sig_acvp = json.loads(resp.content)

    variantFound = False
    for variant in ml_sig_sig_acvp["testGroups"]:
        if variant["parameterSet"] == sig_name:
            variantFound = True
            for testCase in variant["tests"]:
                sk = testCase["sk"]
                # check if its internal API. if yes, check for external mu
                if variant["signatureInterface"] == "internal":
                    if variant["externalMu"]:
                        message = testCase["mu"]
                        extmu = "1"
                    else:
                        message = testCase["message"]
                        extmu = "0"
                else:
                    message = testCase["message"]
                # internal pre-hash API take hash as "message"
                if variant["preHash"] == "preHash":
                    hash_algo = testCase["hashAlg"]
                    message = calc_hash(message, hash_algo)

                signature = testCase["signature"]
                rnd = testCase["rnd"] if not variant["deterministic"] else "0" * 64

                build_dir = helpers.get_current_build_dir_name()
                if variant["signatureInterface"] == "internal":
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigGen_int",
                            sk,
                            message,
                            signature,
                            rnd,
                            extmu
                        ]
                    )
                elif variant["signatureInterface"] == "external" and variant["preHash"] == "pure":
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigGen_ext",
                            sk,
                            message,
                            signature,
                            context,
                            rnd
                        ]
                    )
                elif variant["signatureInterface"] == "external" and variant["preHash"] == "preHash":
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigGen_prehash_ext",
                            sk,
                            message,
                            signature,
                            context,
                            rnd,
                            hash_algo
                        ]
                    )
                else:
                    raise ValueError(
                        f"Unsupported combination: {variant['signatureInterface']}, {variant['preHash']}"
                    )
    assert variantFound == True


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_ml_dsa_sig_ver(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in ml_sig):
        pytest.skip("Not supported")

    resp = cached_requests_get(ml_dsa_ver)
    assert resp.status_code == 200
    ml_sig_sig_acvp = json.loads(resp.content)

    variantFound = False
    for variant in ml_sig_sig_acvp["testGroups"]:
        if variant["parameterSet"] == sig_name:
            variantFound = True
            for testCase in variant["tests"]:
                # check if its internal API. if yes, check for external mu
                if variant["signatureInterface"] == "internal":
                    if variant["externalMu"]:
                        message = testCase["mu"]
                        extmu = "1"
                    else:
                        message = testCase["message"]
                        extmu = "0"
                else:
                    message = testCase["message"]

                # internal pre-hash API take hash as "message"
                if variant["preHash"] == "preHash":
                    hash_algo = testCase["hashAlg"]
                    message = calc_hash(message, hash_algo)

                signature = testCase["signature"]
                pk = testCase["pk"]
                testPassed = "1" if testCase["testPassed"] else "0"

                build_dir = helpers.get_current_build_dir_name()
                if variant["signatureInterface"] == "internal":
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigVer_int",
                            pk,
                            message,
                            signature,
                            testPassed,
                            extmu
                        ]
                    )
                elif variant["signatureInterface"] == "external" and variant["preHash"] == "pure":
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigVer_ext",
                            pk,
                            message,
                            signature,
                            context,
                            testPassed,
                        ]
                    )
                elif variant["signatureInterface"] == "external" and variant["preHash"] == "preHash":
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigVer_prehash_ext",
                            pk,
                            message,
                            signature,
                            context,
                            testPassed,
                            hash_algo
                        ]
                    )
                else:
                    raise ValueError(
                        f"Unsupported combination: {variant['signatureInterface']}, {variant['preHash']}"
                    )

    assert variantFound == True


# TODO: move comments to doc-string where appropriate
# TODO: move this function to `helpers`, maybe?
# SLHDSA tests begin here
def slh_format_name(sig_name):
    # Remove pure tag if applicable
    sig_name = sig_name.replace("PURE_", "")
    # remove prehash tag if applicable
    start = 8
    end = sig_name.find("PREHASH_") + 8
    if end >= 8:
        sig_name = sig_name[:start] + sig_name[end:]
    # use dashes
    sig_name = sig_name.replace("_", "-")
    # lowercase param set indicator
    sig_name = sig_name[:-1] + sig_name[-1].lower()

    return sig_name


# Essentially inverse of above function for a test case
def slh_test_sig_name(variant, testCase):
    sig_name = variant["parameterSet"]
    sig_name = sig_name.replace("-", "_")
    sig_name = sig_name[:-1] + sig_name[-1].upper()

    if variant["preHash"] != "preHash":
        sig_name = sig_name[:7] + "_PURE" + sig_name[7:]
    else:
        hashAlg = testCase["hashAlg"].replace("-", "_")
        sig_name = sig_name[:7] + "_" + hashAlg + "_PREHASH" + sig_name[7:]

    return sig_name


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_slh_dsa_sig_keygen(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in slh_sig):
        pytest.skip("Not supported")

    acvp_sig_name = slh_format_name(sig_name)

    resp = cached_requests_get(slh_dsa_kg)
    assert resp.status_code == 200
    slh_sig_kg_acvp = json.loads(resp.content)

    variantFound = False
    for variant in slh_sig_kg_acvp["testGroups"]:
        if variant["parameterSet"] == acvp_sig_name:
            variantFound = True
            for testCase in variant["tests"]:
                skSeed = testCase["skSeed"]
                skPrf = testCase["skPrf"]
                pkSeed = testCase["pkSeed"]
                pk = testCase["pk"]
                sk = testCase["sk"]

                build_dir = helpers.get_current_build_dir_name()
                helpers.run_subprocess(
                    [
                        f"{build_dir}/tests/vectors_sig",
                        sig_name,
                        "keyGen",
                        skSeed,
                        skPrf,
                        pkSeed,
                        pk,
                        sk,
                    ]
                )

    assert variantFound == True


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_slh_dsa_sig_gen(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in slh_sig):
        pytest.skip("Not supported")

    resp = cached_requests_get(slh_dsa_sig)
    assert resp.status_code == 200
    slh_sig_sig_acvp = json.loads(resp.content)

    for variant in slh_sig_sig_acvp["testGroups"]:
        for testCase in variant["tests"]:
            if slh_test_sig_name(variant, testCase) == sig_name:
                sk = testCase["sk"]
                message = testCase["message"]
                signature = testCase["signature"]

                rnd = (
                    testCase["additionalRandomness"]
                    if not variant["deterministic"]
                    else ""
                )

                build_dir = helpers.get_current_build_dir_name()
                if variant["signatureInterface"] == "internal":
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigGen_int",
                            sk,
                            message,
                            signature,
                            rnd,
                            "0"
                        ]
                    )
                else:
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigGen_ext",
                            sk,
                            message,
                            signature,
                            context,
                            rnd,
                        ]
                    )


@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize("sig_name", helpers.available_sigs_by_name())
def test_acvp_vec_slh_dsa_sig_ver(sig_name):

    if not (helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip("Not enabled")
    if not (sig_name in slh_sig):
        pytest.skip("Not supported")

    resp = cached_requests_get(slh_dsa_ver)
    assert resp.status_code == 200
    slh_sig_sig_acvp = json.loads(resp.content)

    for variant in slh_sig_sig_acvp["testGroups"]:
        for testCase in variant["tests"]:
            if slh_test_sig_name(variant, testCase) == sig_name:
                message = testCase["message"]
                signature = testCase["signature"]
                pk = testCase["pk"]
                testPassed = "1" if testCase["testPassed"] else "0"

                build_dir = helpers.get_current_build_dir_name()
                if variant["signatureInterface"] == "internal":
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigVer_int",
                            pk,
                            message,
                            signature,
                            testPassed,
                            "0"
                        ]
                    )
                else:
                    context = testCase["context"]
                    helpers.run_subprocess(
                        [
                            f"{build_dir}/tests/vectors_sig",
                            sig_name,
                            "sigVer_ext",
                            pk,
                            message,
                            signature,
                            context,
                            testPassed,
                        ]
                    )


if __name__ == "__main__":
    import sys

    pytest.main(sys.argv)
