#ifndef HIRA_MATH_MODULAR_H
#define HIRA_MATH_MODULAR_H

#include "hira/common_header.h"
#include "hira/math/modular/binom.h"
#include "hira/math/modular/mod_int.h"
#include "hira/math/prime/miller_rabin.h"

namespace hira {

namespace math {

template <i32 mod_>
class Lucas {
 public:
  using mod_int_t = ModInt<i32, mod_, i64>;

  template <typename ValueType>
  static mod_int_t binom(ValueType n, ValueType m) {
    // mod_ should be prime!
    // ASSERT(MillerRabinTest(mod_));

    if (n < 0 || m < 0 || m > n)
      return 0;

    if (m == 0 || m == n)
      return 1;

    if (n < mod_)
      return math::binom<mod_int_t>(n, m);

    return Lucas::binom(n % mod_, m % mod_) * Lucas::binom(n / mod_, m / mod_);
  }
};

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_MODULAR_H