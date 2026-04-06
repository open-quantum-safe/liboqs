#ifndef PQCHQC_H
#define PQCHQC_H

#ifndef PQCHQC_NAMESPACE_PREFIX
#define PQCHQC_NAMESPACE_PREFIX
#endif

#define PQCHQC_CONCAT_(lhs, rhs) lhs##rhs
#define PQCHQC_CONCAT(lhs, rhs) PQCHQC_CONCAT_(lhs, rhs)

#define PQCHQC_NAMESPACE(name) PQCHQC_CONCAT(PQCHQC_NAMESPACE_PREFIX, name)

#endif /* !PQCHQC_H */
