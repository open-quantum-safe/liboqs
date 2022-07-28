# SPDX-License-Identifier: MIT

import helpers
import pytest
import sys
from threading import Thread

# Stateful signature schemes testing (multithreaded)
all_sig_stfls = helpers.available_sig_stfls_by_name()
all_threads = []

print("All the signature schemes are here :\n")
print(all_sig_stfls)

# Instantiate threads
for sig_stfl in all_sig_stfls:
    thread = Thread(target=helpers.run_subprocess, args=([helpers.path_to_executable('test_sig_stfl'), sig_stfl],))
    all_threads.append(thread)

# Start threads for each signature scheme
for num in range(len(all_threads)):
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl)):
        pytest.skip('Not enabled')
    
    print("=== Starting ", all_sig_stfls[num], " ===")
    all_threads[num].start()

for thread in all_threads:
    thread.join()

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

