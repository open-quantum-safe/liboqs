/**
 * @name Unchecked OpenSSL EVP return value
 * @description Calls to OpenSSL EVP functions whose status code is discarded
 *              silently ignore OpenSSL errors. Check the result, normally
 *              with the OQS_OPENSSL_GUARD macro.
 * @kind problem
 * @problem.severity warning
 * @precision high
 * @id liboqs/openssl-unchecked-return
 * @tags security
 *       correctness
 */

import cpp

from FunctionCall call, Function f
where
  f = call.getTarget() and
  f.getName().matches("EVP%") and
  not f.getUnspecifiedType() instanceof PointerType and
  not f.getUnspecifiedType() instanceof VoidType and
  // The status code is thrown away. Tested (e.g. by OQS_OPENSSL_GUARD), stored,
  // or explicitly discarded with a (void) cast are all accepted.
  call instanceof ExprInVoidContext
select call, "Return value of " + f.getName() + "() is discarded; check it with OQS_OPENSSL_GUARD."
