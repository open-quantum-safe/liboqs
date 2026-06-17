/**
 * @file namespace.h
 * @brief C preprocessing macro for namespacing
 */

#ifndef HQC_NAMESPACE_H
#define HQC_NAMESPACE_H

#ifndef PQCHQC_NAMESPACE_PREFIX
#warning "PQCHQC_NAMESPACE_PREFIX is not defined, default to no namespace"
#define PQCHQC_NAMESPACE_PREFIX
#endif

#define PQCHQC_CONCAT_(lhs, rhs) lhs##rhs
#define PQCHQC_CONCAT(lhs, rhs) PQCHQC_CONCAT_(lhs, rhs)

#define PQCHQC_NAMESPACE(name) PQCHQC_CONCAT(PQCHQC_NAMESPACE_PREFIX, name)

#endif /* !HQC_NAMESPACE_H */
