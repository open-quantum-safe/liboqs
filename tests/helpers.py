import os
import os.path
import subprocess

# subprocess.run is not defined on older versions of Python that are present on our test platform
# so we need to supply our own backport
# see https://stackoverflow.com/a/40590445
def run(*popenargs, input=None, check=False, **kwargs):
    if input is not None:
        if 'stdin' in kwargs:
            raise ValueError('stdin and input arguments may not both be used.')
        kwargs['stdin'] = subprocess.PIPE

    process = subprocess.Popen(*popenargs, **kwargs)
    try:
        stdout, stderr = process.communicate(input)
    except:
        process.kill()
        process.wait()
        raise
    retcode = process.poll()
    if check and retcode:
        raise subprocess.CalledProcessError(
            retcode, process.args, output=stdout, stderr=stderr)
    return retcode, stdout, stderr

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
    # then print it, which nose/unittest will then capture and
    # buffer appropriately
    print(working_dir + " > " + " ".join(command))
    retcode, stdout, stderr = run(
        command,
        input=input,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=working_dir,
        env=env,
    )
    print(stdout.decode('utf-8'))
    assert retcode == expected_returncode, "Got unexpected return code {}".format(retcode)
    return stdout.decode('utf-8')

def available_kems_by_name():
    available_names = []
    with open(os.path.join('include', 'oqs', 'kem.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_KEM_alg_"):
                kem_name = line.split(' ')[2]
                kem_name = kem_name[1:-2]
                available_names.append(kem_name)
    return available_names

def is_kem_enabled_by_name(name):
    symbol = None
    with open(os.path.join('include', 'oqs', 'kem.h')) as fh:
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
    with open(os.path.join('include', 'oqs', 'oqsconfig.h')) as fh:
        for line in fh:
            if line.startswith("#define OQS_ENABLE_KEM_"):
                kem_symbol = line.split(' ')[1]
                kem_symbol = kem_symbol[len("OQS_ENABLE_KEM_"):]
                if kem_symbol == symbol:
                    return True
    return False
