import oqs
import platform  # to learn the OS we're on
import random

# sigs for which unit testing is disabled
disabled_sig_patterns = []

if platform.system() == "Windows":
    disabled_sig_patterns = ["Rainbow-IIIc", "Rainbow-Vc"]


def test_correctness():
    for alg_name in oqs.get_enabled_sig_mechanisms():
        if any(item in alg_name for item in disabled_sig_patterns):
            continue
        yield check_correctness, alg_name


def check_correctness(alg_name):
    message = bytes(random.getrandbits(8) for _ in range(100))
    sig = oqs.Signature(alg_name)
    public_key = sig.generate_keypair()
    signature = sig.sign(message)
    assert sig.verify(message, signature, public_key)
    sig.free()


def test_wrong_message():
    for alg_name in oqs.get_enabled_sig_mechanisms():
        if any(item in alg_name for item in disabled_sig_patterns):
            continue
        yield check_wrong_message, alg_name


def check_wrong_message(alg_name):
    message = bytes(random.getrandbits(8) for _ in range(100))
    sig = oqs.Signature(alg_name)
    public_key = sig.generate_keypair()
    signature = sig.sign(message)
    wrong_message = bytes(random.getrandbits(8) for _ in range(100))
    assert not (sig.verify(wrong_message, signature, public_key))
    sig.free()


def test_wrong_signature():
    for alg_name in oqs.get_enabled_sig_mechanisms():
        if any(item in alg_name for item in disabled_sig_patterns):
            continue
        yield check_wrong_signature, alg_name


def check_wrong_signature(alg_name):
    message = bytes(random.getrandbits(8) for _ in range(100))
    sig = oqs.Signature(alg_name)
    public_key = sig.generate_keypair()
    signature = sig.sign(message)
    wrong_signature = bytes(random.getrandbits(8) for _ in range(sig.details['length_signature']))
    assert not (sig.verify(message, wrong_signature, public_key))
    sig.free()


def test_wrong_public_key():
    for alg_name in oqs.get_enabled_sig_mechanisms():
        if any(item in alg_name for item in disabled_sig_patterns):
            continue
        yield check_wrong_public_key, alg_name


def check_wrong_public_key(alg_name):
    message = bytes(random.getrandbits(8) for _ in range(100))
    sig = oqs.Signature(alg_name)
    public_key = sig.generate_keypair()
    signature = sig.sign(message)
    wrong_public_key = bytes(random.getrandbits(8) for _ in range(sig.details['length_public_key']))
    assert not (sig.verify(message, signature, wrong_public_key))
    sig.free()


def test_not_supported():
    try:
        sig = oqs.Signature("bogus")
        raise AssertionError("oqs.MechanismNotSupportedError was not raised.")
    except oqs.MechanismNotSupportedError:
        pass
    except E:
        raise AssertionError("An unexpected exception was raised. " + E)


def test_not_enabled():
    # TODO: test broken as the compiled lib determines which algorithms are supported and enabled
    for alg_name in oqs.get_supported_sig_mechanisms():
        if alg_name not in oqs.get_enabled_sig_mechanisms():
            # found a non-enabled but supported alg
            try:
                sig = oqs.Signature(alg_name)
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
