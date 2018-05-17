This is the implementation which is used to give high
level optimizations.

Please see the README file in the Reference_Implementation
directory for major details re usage

The optimizations implemented in this version are
  - modp used Montgomery
  - FFT uses inplace FFT algorithm (no recursion, less copying, less memory)
  - Various optimizations re delayed reduction and access to the XOF etc

