/**
 * @name Unchecked OQS_MEM allocation
 * @description The result of an OQS_MEM_* allocation is used before it is tested
 *              against NULL, or never tested. Check it first, normally with
 *              OQS_EXIT_IF_NULLPTR, or add the
 *              comment "IGNORE malloc-check" to the allocation's line.
 * @kind problem
 * @problem.severity warning
 * @precision high
 * @id liboqs/unchecked-allocation
 * @tags security
 *       correctness
 */

import cpp
import semmle.code.cpp.valuenumbering.HashCons

/**
 * Gets a path pattern (as for `matches`) for imported code that is not
 * checked, because it has to be fixed upstream.
 */
string mallocCheckIgnoredDir() {
  // CROSS: unchecked allocations of discarded buffers in the AVX2 CSPRNG, see
  // https://github.com/CROSS-signature/CROSS-lib-oqs/blob/ce9933c7023a7aace4ac05b779995334a9996e6a/generate/crypto_sign/cross-rsdp-128-fast/avx2/csprng_hash.h#L105-L107
  result = "src/sig/cross/upcross_%_avx2/csprng_hash.h"
}

class OqsAllocation extends FunctionCall {
  OqsAllocation() {
    this.getTarget()
        .hasGlobalName([
            "OQS_MEM_malloc", "OQS_MEM_calloc", "OQS_MEM_strdup", "OQS_MEM_aligned_alloc"
          ])
  }
}

/** Gets an expression whose value may be the result of `call`, e.g. `c ? call : NULL`. */
Expr allocatedValue(OqsAllocation call) {
  result = call
  or
  exists(ConditionalExpr c | c = result |
    c.getThen() = allocatedValue(call) or c.getElse() = allocatedValue(call)
  )
}

/** Holds if `e` is compared with NULL, negated, or used directly as a condition. */
predicate isNullTest(Expr e) {
  exists(EqualityOperation eq |
    eq.getAnOperand() = e and
    eq.getAnOperand().getValue() = "0"
  )
  or
  exists(NotExpr n | n.getOperand() = e)
  or
  e.getFullyConverted().isCondition()
}

/**
 * Holds if `e` accesses the pointer that `call`'s result is stored in: the
 * variable it initializes, or the lvalue it is assigned to (e.g. `s->buf[i]`).
 */
predicate isAccessToResult(OqsAllocation call, Expr e) {
  exists(Variable v |
    v.getInitializer().getExpr() = allocatedValue(call) and
    e.(VariableAccess).getTarget() = v
  )
  or
  exists(AssignExpr a |
    a.getRValue() = allocatedValue(call) and
    hashCons(e) = hashCons(a.getLValue()) and
    e.getEnclosingFunction() = call.getEnclosingFunction()
  )
}

/** Holds if `e` is the argument to a function that frees memory, which accepts NULL. */
predicate isFreed(Expr e) {
  exists(FunctionCall free |
    free.getTarget()
        .hasGlobalName([
            "free", "OQS_MEM_insecure_free", "OQS_MEM_secure_free", "OQS_MEM_aligned_free",
            "OQS_MEM_aligned_secure_free"
          ]) and
    free.getArgument(0) = e
  )
}

/**
 * Holds if `e` uses `call`'s result for something other than testing it
 * against NULL, overwriting it, or freeing it.
 */
predicate isUse(OqsAllocation call, Expr e) {
  isAccessToResult(call, e) and
  not isNullTest(e) and
  not e = any(AssignExpr w).getLValue() and
  not isFreed(e)
}

/** Holds if control flow past `n` no longer holds `call`'s unchecked result. */
predicate isBarrier(OqsAllocation call, ControlFlowNode n) {
  // it is tested against NULL
  isAccessToResult(call, n) and isNullTest(n)
  or
  // it is tested anywhere in a condition like `!a || !b`: short-circuiting
  // can skip the test, but only once another check has failed
  exists(BinaryLogicalOperation cond, Expr test | cond = n |
    not cond.getParent() instanceof BinaryLogicalOperation and
    test.getParent+() = cond and
    isAccessToResult(call, test) and
    isNullTest(test)
  )
  or
  // it is overwritten
  exists(AssignExpr w | w = n |
    isAccessToResult(call, w.getLValue()) and
    not w.getRValue() = allocatedValue(call)
  )
}

/** Gets a node that can run after `call` without `call`'s result having been tested. */
ControlFlowNode reachableUnchecked(OqsAllocation call) {
  result = call.getASuccessor()
  or
  exists(ControlFlowNode prev |
    prev = reachableUnchecked(call) and
    not isBarrier(call, prev) and
    result = prev.getASuccessor()
  )
}

/** Holds if the result of `call` is tested against NULL somewhere after the call. */
predicate isTested(OqsAllocation call) {
  exists(Expr test |
    isAccessToResult(call, test) and
    isNullTest(test) and
    test = call.getASuccessor+()
  )
}

/** Holds if `call`'s result needs no check here, because it is tested or returned at once. */
predicate isImmediatelyHandled(OqsAllocation call) {
  // if ((x = OQS_MEM_malloc(n)) == NULL)
  exists(AssignExpr a | a.getRValue() = allocatedValue(call) and isNullTest(a))
  or
  // return OQS_MEM_malloc(n); leaves the check to the caller
  exists(ReturnStmt r | r.getExpr() = allocatedValue(call))
}

/** Holds if `call`'s line opts out with an "IGNORE malloc-check" comment. */
predicate isIgnored(OqsAllocation call) {
  exists(Comment c |
    c.getFile() = call.getFile() and
    c.getLocation().getStartLine() = call.getLocation().getStartLine() and
    c.getContents().matches("%IGNORE malloc-check%")
  )
}

from OqsAllocation call, string problem
where
  not isImmediatelyHandled(call) and
  not isIgnored(call) and
  not call.getFile().getRelativePath().matches(mallocCheckIgnoredDir()) and
  (
    not isTested(call) and
    problem = "is not checked for NULL"
    or
    isTested(call) and
    exists(int line |
      line =
        min(Expr use |
          isUse(call, use) and use = reachableUnchecked(call)
        |
          use.getLocation().getStartLine()
        ) and
      problem = "is used on line " + line + " before it is checked for NULL"
    )
  )
select call,
  "Result of " + call.getTarget().getName() + "() " + problem +
    ". Check it first (e.g. with OQS_EXIT_IF_NULLPTR), or if this is intended, add the comment " +
    "'// IGNORE malloc-check' to this line or a path pattern to mallocCheckIgnoredDir() in " +
    ".github/codeql/queries/malloc-null-check.ql."
