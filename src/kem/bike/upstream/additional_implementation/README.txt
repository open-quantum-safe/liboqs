To compile:
    make

Default is non constant time BIKE1 in lEVEL=5 (128-bit quantum security) with AVX2 support.

Additional compilation flags:
 - USE_NIST_RAND - Using the RDBG of NIST.
 - FIXED_SEED    - Using a fixed seed.
 - RDTSC         - Measure time in cycles not in mseconds
 - VERBOSE       - Add verbose (level1-3)
 - CHECK_COMPILATION - To validate that there are no compiler warnings.
 - CONSTANT_TIME - Compile the program in constant time mode (slower).
 - NUM_OF_TESTS  - Set the number of tests to be run.
 - AVX512        - Compile with AVX512 support.
 - VALIDATE_CONSTANT_TIME - Compile a non constant time versions that generate inputs compatible with the constant time implementaiton.
 - LEVEL         - Security level (1/3/5).
 - BIKE_VER      - Choose the BIKE to be used (1/2/3).
 - BATCH_SIZE    - Only in BIKE2, set the batch key generation optimization.
 
 To clean:
    make clean.
    
Note: A clean is required whenever different flags are used for compilation.