import oqs
import platform  # to learn the OS we're on
import random

# KEMs for which unit testing is disabled
disabled_KEM_patterns = []

if platform.system() == "Windows":
    disabled_KEM_patterns = ["Classic-McEliece"]


def test_correctness():
    for alg_name in oqs.get_enabled_KEM_mechanisms():
        if any(item in alg_name for item in disabled_KEM_patterns):
            continue
        yield check_correctness, alg_name


def check_correctness(alg_name):
    kem = oqs.KeyEncapsulation(alg_name)
    public_key = kem.generate_keypair()
    ciphertext, shared_secret_server = kem.encap_secret(public_key)
    shared_secret_client = kem.decap_secret(ciphertext)
    assert shared_secret_client == shared_secret_server
    kem.free()


def test_wrong_ciphertext():
    for alg_name in oqs.get_enabled_KEM_mechanisms():
        if any(item in alg_name for item in disabled_KEM_patterns):
            continue
        yield check_wrong_ciphertext, alg_name


def check_wrong_ciphertext(alg_name):
    kem = oqs.KeyEncapsulation(alg_name)
    public_key = kem.generate_keypair()
    ciphertext, shared_secret_server = kem.encap_secret(public_key)
    wrong_ciphertext = bytes(random.getrandbits(8) for _ in range(kem.details['length_ciphertext']))
    shared_secret_client = kem.decap_secret(wrong_ciphertext)
    assert shared_secret_client != shared_secret_server
    kem.free()


def test_not_supported():
    try:
        kem = oqs.KeyEncapsulation("bogus")
        raise AssertionError("oqs.MechanismNotSupportedError was not raised.")
    except oqs.MechanismNotSupportedError:
        pass
    except E:
        raise AssertionError("An unexpected exception was raised. " + E)


def test_not_enabled():
    # TODO: test broken as the compiled lib determines which algorithms are supported and enabled
    for alg_name in oqs.get_supported_KEM_mechanisms():
        if alg_name not in oqs.get_enabled_KEM_mechanisms():
            # found a non-enabled but supported alg
            try:
                kem = oqs.KeyEncapsulation(alg_name)
                raise AssertionError("oqs.MechanismNotEnabledError was not raised.")
            except oqs.MechanismNotEnabledError:
                pass
            except E:
                raise AssertionError("An unexpected exception was raised. " + E)


if __name__ == '__main__':
    try:
        import nose2

        nose2.main()

    except ImportError:
        import nose

        nose.runmodule()
