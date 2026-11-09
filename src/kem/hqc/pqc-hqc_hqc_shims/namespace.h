#ifndef HQC_NAMESPACE_H
#define HQC_NAMESPACE_H

#define HQC_CONCAT_(x, y) x##y
#define HQC_CONCAT(x, y) HQC_CONCAT_(x, y)
#define HQC_NS(sym) HQC_CONCAT(HQC_CONCAT(HQC_NAMESPACE_PREFIX, _), sym)

#endif /* !HQC_NAMESPACE_H */
