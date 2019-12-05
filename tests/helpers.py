import functools
import os
import os.path
import pytest
import subprocess
import sys

def run_subprocess(command, working_dir='.', env=None, expected_returncode=0, input=None):
    """
    Helper function to run a shell command and report success/failure
    depending on the exit status of the shell command.
    """
    if env is not None:
        env_ = os.environ.copy()
        env_.update(env)
        env = env_

    # Note we need to capture stdout/stderr from the subprocess,
    # then print it, which pytest will then capture and
    # buffer appropriately
    print(working_dir + " > " + " ".join(command))
    result = subprocess.run(
        command,
        input=input,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=working_dir,
        env=env,
    )
    if result.returncode != expected_returncode:
        print(result.stdout.decode('utf-8'))
        assert False, "Got unexpected return code {}".format(result.returncode)
    return result.stdout.decode('utf-8')

def available_kems_by_name():
    available_names = []
    with open(os.path.join('src', 'kem', 'kem.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_KEM_alg_"):
                kem_name = line.split(' ')[2]
                kem_name = kem_name[1:-2]
                if kem_name != "DEFAULT":
                    available_names.append(kem_name)
    return available_names

def is_kem_enabled_by_name(name):
    symbol = None
    with open(os.path.join('src', 'kem', 'kem.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_KEM_alg_"):
                kem_symbol = line.split(' ')[1]
                kem_symbol = kem_symbol[len("OQS_KEM_alg_"):]
                kem_name = line.split(' ')[2]
                kem_name = kem_name[1:-2]
                if kem_name == name:
                    symbol = kem_symbol
                    break
    if symbol == None: return False
    if sys.platform.startswith("win"):
        header = os.path.join('VisualStudio', 'winconfig.h')
    else:
        header = os.path.join('include', 'oqs', 'oqsconfig.h')
    with open(header) as fh:
        for line in fh:
            if line.startswith("#define OQS_ENABLE_KEM_"):
                kem_symbol = line.split(' ')[1]
                kem_symbol = kem_symbol[len("OQS_ENABLE_KEM_"):].rstrip()
                if kem_symbol == symbol:
                    return True
    return False

def available_sigs_by_name():
    available_names = []
    with open(os.path.join('src', 'sig', 'sig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_SIG_alg_"):
                sig_name = line.split(' ')[2]
                sig_name = sig_name[1:-2]
                if sig_name != "DEFAULT":
                    available_names.append(sig_name)
    return available_names

def is_sig_enabled_by_name(name):
    symbol = None
    with open(os.path.join('src', 'sig', 'sig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_SIG_alg_"):
                sig_symbol = line.split(' ')[1]
                sig_symbol = sig_symbol[len("OQS_SIG_alg_"):]
                sig_name = line.split(' ')[2]
                sig_name = sig_name[1:-2]
                if sig_name == name:
                    symbol = sig_symbol
                    break
    if symbol == None: return False
    if sys.platform.startswith("win"):
        header = os.path.join('VisualStudio', 'winconfig.h')
    else:
        header = os.path.join('include', 'oqs', 'oqsconfig.h')
    with open(header) as fh:
        for line in fh:
            if line.startswith("#define OQS_ENABLE_SIG_"):
                sig_symbol = line.split(' ')[1]
                sig_symbol = sig_symbol[len("OQS_ENABLE_SIG_"):].rstrip()
                if sig_symbol == symbol:
                    return True
    return False

def filtered_test(func):
    funcname = func.__name__[len("test_"):]

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if ('SKIP_TESTS' in os.environ) and (funcname in os.environ['SKIP_TESTS'].lower().split(',')):
            pytest.skip("Test disabled by filter")
        else:
            return func(*args, **kwargs)
    return wrapper

def path_to_executable(program_name):
    if sys.platform.startswith("win"):
        if 'APPVEYOR_BUILD_FOLDER' not in os.environ: os.environ['APPVEYOR_BUILD_FOLDER'] = "."
        if 'PLATFORM' not in os.environ: os.environ['PLATFORM'] = "x64"
        if 'CONFIGURATION' not in os.environ: os.environ['CONFIGURATION'] = "Debug"
        return os.path.join(
            os.environ['APPVEYOR_BUILD_FOLDER'],
            'VisualStudio',
            os.environ['PLATFORM'],
            os.environ['CONFIGURATION'],
            program_name + ".EXE"
        )
    else:
        return os.path.join("tests", program_name)

def available_use_options_by_name():
    enabled_use_options = []
    if sys.platform.startswith("win"):
        # Windows doesn't have access to the options config file, so return an empty list
        return enabled_use_options
    with open(os.path.join('include', 'oqs', 'oqsconfig.h')) as fh:
        for line in fh:
            if line.startswith("#define USE_") and line.endswith("1\n"):
                option_name = line.split(' ')[1][len("USE_"):-1]
                enabled_use_options.append(option_name)
    return enabled_use_options

def is_use_option_enabled_by_name(name):
    return name in available_use_options_by_name()
