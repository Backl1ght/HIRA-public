#ifndef HIRA_MATH_BIT_REVERSAL_TRANSFORM_H
#define HIRA_MATH_BIT_REVERSAL_TRANSFORM_H

#include "hira/common_header.h"

namespace hira {

namespace math {

template <typename ValueType>
void BitReversalTransform(std::vector<ValueType>& a) {
  int n = a.size();

  // n should power of 2.
  ASSERT((n & (n - 1)) == 0);

  std::vector<int> r(n);
  r[0] = 0;
  for (int i = 1; i < n; ++i)
    r[i] = (r[i >> 1] >> 1) | (i & 1 ? (n >> 1) : 0);

  for (int i = 1; i < n; ++i)
    if (i < r[i])
      std::swap(a[i], a[r[i]]);
}

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_BIT_REVERSAL_H