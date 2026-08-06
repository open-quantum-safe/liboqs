/**
 * @name Unchecked OpenSSL EVP return value
 * @description Calls to OpenSSL EVP functions whose return value is not
 *              checked by the OQS_OPENSSL_GUARD macro may silently ignore
 *              errors, leading to undefined behaviour.
 * @kind problem
 * @problem.severity warning
 * @precision medium
 * @id cpp/openssl-unchecked-return
 * @tags security
 *       correctness
 */

import cpp

from FunctionCall call, Function f
where
  f = call.getTarget() and
  f.getName().matches("EVP%") and
  not f.getType() instanceof PointerType and
  not f.getType() instanceof VoidType and
  not exists(MacroAccess m |
    m.getLocation().subsumes(call.getLocation()) and
    m.getMacroName() = "OQS_OPENSSL_GUARD"
  )
select call, "Return value of " + f.getName() + "() is not checked by OQS_OPENSSL_GUARD."
