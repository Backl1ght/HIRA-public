#ifndef HIRA_MATH_MODULAR_BINOM_H
#define HIRA_MATH_MODULAR_BINOM_H

#include "hira/common_header.h"

#include "hira/math/modular/factorial.h"

namespace hira {

namespace math {

template <typename ModInt>
ModInt binom(int n, int m) {
  if (n < 0 || m < 0 || m > n)
    return ModInt(0);

  return fact<ModInt>(n) * inv_fact<ModInt>(m) * inv_fact<ModInt>(n - m);
}

// const auto binom = math::binom<math::ModInt998>;

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_MODULAR_BINOM_H