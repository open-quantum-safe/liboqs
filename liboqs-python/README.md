liboqs-python: Python 3 bindings for liboqs
===========================================

[![Build status - CircleCI Linux](https://circleci.com/gh/open-quantum-safe/liboqs-python.svg?style=svg)](https://circleci.com/gh/open-quantum-safe/liboqs-python)
[![Build status](https://ci.appveyor.com/api/projects/status/jjo1ti9l5e0grgln?svg=true)](https://ci.appveyor.com/project/vsoftco/liboqs-python)

---

**liboqs-python** offers a Python module providing quantum-resistant cryptographic algorithms via liboqs.

Overview
--------

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms. See more about liboqs at [https://github.com/open-quantum-safe/liboqs/](https://github.com/open-quantum-safe/liboqs/), including a list of supported algorithms.

**liboqs-python** is an open source Python 3 wrapper for the liboqs C library. liboqs-python provides:

- a common API for post-quantum key encapsulation mechanisms and digital signature schemes
- a collection of open source implementations of post-quantum cryptography algorithms

The OQS project also provides prototype integrations into application-level protocols to enable testing of quantum-resistant cryptography.

More information on OQS can be found on our website: [https://openquantumsafe.org/](https://openquantumsafe.org/).

Pre-requisites
--------------
Python 3.x
liboqs-python depends on the [liboqs](https://github.com/open-quantum-safe/liboqs) C library; liboqs must first be compiled as a Linux/macOS/Windows library (i.e. using `ninja install` with `-DBUILD_SHARED_LIBS=ON` during configuration), see the specific platform building instructions below.

Contents
--------

The project contains the following files:

 - **`oqs/oqs.py`: a Python 3 module wrapper for the liboqs C library.**
 - `oqs/rand.py`: a Python 3 module supporting RNGs from `<oqs/rand.h>`
 - `examples/kem.py`: key encapsulation example
 - `examples/rand.py`: RNG example
 - `examples/sig.py`: signature example
 - `tests`: unit tests

Usage
-----

liboqs-python defines two main classes: `KeyEncapsulation` and `Signature`, providing post-quantum key encapsulation and signture mechanisms, respectively. Each must be instantiated with a string identifying one of mechanisms supported by liboqs; these can be enumerated using the `get_enabled_KEM_mechanisms` and `get_enabled_sig_mechanisms` functions. The files in `examples/` demonstrate the wrapper's API.
Support for alternative RNGs is provided via the `randombytes[*]` functions.

Installation
------------

First, you must build liboqs according to the [liboqs building instructions](https://github.com/open-quantum-safe/liboqs#linuxmacos) 
with shared library support enabled (add `-DBUILD_SHARED_LIBS=ON` to the `cmake` command), followed (optionally) by a `sudo ninja install` 
to ensure that the shared library is visible system-wide (by default it installs under `/usr/local/include` and `/usr/local/lib` on Linux/macOS). 

On Linux/macOS you may need to set the `LD_LIBRARY_PATH` (`DYLD_LIBRARY_PATH` on macOS) environment variable to point to the path to liboqs' library directory, e.g.

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

assuming `liboqs.so.*` were installed in `/usr/local/lib` (true if you ran `sudo ninja install` after building liboqs).

On Windows ensure that the liboqs shared library `oqs.dll` is visible system-wide. Use the "Edit the system environment variables" Control Panel 
tool or type in a Command Prompt
	
	set PATH="%PATH%;C:\some\dir\liboqs\build\bin"
	
of course replacing the paths with the ones corresponding to your system.

liboqs-python does not depend on any other Python packages. The package isn't hosted on PyPI yet. We recommend to install 
it into a virtualenv using:

	# create & activate virtual environment, e.g.:
	python3 -m venv <virtualenv_name>
	source <virtualenv_name>/bin/activate
	
	cd /some/dir/liboqs-python
	python3 setup.py install

On Windows replace the command `source <virtualenv_name>/bin/activate` with `<virtualenv_name>\Scripts\activate.bat`.

Running
-------

The liboqs-python project should be in the `PYTHONPATH`:

	export PYTHONPATH=/some/dir/liboqs-python
	
or, on Windows platforms, use the "Edit the system environment variables" Control Panel 
tool or type in a Command Prompt

    set PYTHONPATH="C:\some\dir\liboqs-python"

As any python module, liboqs wrapper components can be imported into python programs with `import oqs`.

To run an example program:

	python3 examples/kem.py

To run the unit tests with a test runner (e.g. nose or rednose (`apt install python3-nose python3-rednose` or `pip3 install nose rednose`)):

	python3 -m nose --rednose --verbose

To run the unit tests without a test runner:

	python3 tests/test_kem.py
	python3 tests/test_sig.py

The module has been tested using Python 3 on Linux Debian 10 (Buster), Linux Ubuntu 18.04 (Bionic), macOS 10.14.3, 
and Windows 10. We run continuous integration tests on CircleCI on Linux Debian 10 (Buster) and Linux Ubuntu 18.04 (Bionic) on x86_64,
and AppVeyor on Windows. 

Limitations and security
------------------------

liboqs is designed for prototyping and evaluating quantum-resistant cryptography. Security of proposed quantum-resistant algorithms may rapidly change as research advances, and may ultimately be completely insecure against either classical or quantum computers.

We believe that the NIST Post-Quantum Cryptography standardization project is currently the best avenue to identifying potentially quantum-resistant algorithms. liboqs does not intend to "pick winners", and we strongly recommend that applications and protocols rely on the outcomes of the NIST standardization project when deploying post-quantum cryptography.

We acknowledge that some parties may want to begin deploying post-quantum cryptography prior to the conclusion of the NIST standardization project. We strongly recommend that any attempts to do make use of so-called **hybrid cryptography**, in which post-quantum public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

Just like liboqs, liboqs-python is provided "as is", without warranty of any kind. See [LICENSE.txt](https://github.com/open-quantum-safe/liboqs-python/blob/master/LICENSE.txt) for the full disclaimer.

License
-------

liboqs-python is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs-python/blob/master/LICENSE.txt) for details.

Team
----

The Open Quantum Safe project is led by [Douglas Stebila](https://www.douglas.stebila.ca/research/) and [Michele Mosca](http://faculty.iqc.uwaterloo.ca/mmosca/) at the University of Waterloo.

### Contributors

Contributors to the liboqs-python wrapper include:

- Ben Davies (University of Waterloo)
- Vlad Gheorghiu (evolutionQ, University of Waterloo)
- Christian Paquin (Microsoft Research)
- Douglas Stebila (University of Waterloo)

### Support

Financial support for the development of Open Quantum Safe has been provided by Amazon Web Services and the Tutte Institute for Mathematics and Computing.

We'd like to make a special acknowledgement to the companies who have dedicated programmer time to contribute source code to OQS, including Amazon Web Services, Cisco Systems, evolutionQ, IBM Research, and Microsoft Research.

Research projects which developed specific components of OQS have been supported by various research grants, including funding from the Natural Sciences and Engineering Research Council of Canada (NSERC); see the source papers for funding acknowledgments.
