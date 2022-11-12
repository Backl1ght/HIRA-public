#ifndef HIRA_MATH_PRIME_MILLER_RABIN_H
#define HIRA_MATH_PRIME_MILLER_RABIN_H

#include "hira/common_header.h"

#include "hira/math/modular/mod_functions.h"
#include "hira/misc/i128.h"

namespace hira {

namespace math {

// Time Complexity: $O(k \log^{3} n)$
bool MillerRabinTest(int64_t n) {
  // Strong enough for $n < 2^64$, see https://oeis.org/A014233.
  constexpr static int kTestRounds = 12;
  constexpr static int kTestBase[kTestRounds] = {2,  3,  5,  7,  11, 13,
                                                 17, 19, 23, 29, 31, 37};

  if (n <= kTestBase[kTestRounds - 1]) {
    return *std::lower_bound(kTestBase, kTestBase + kTestRounds, n) == n;
  }

  int64_t d = n - 1, r = 0;
  while (d % 2 == 0) {
    d >>= 1;
    r = r + 1;
  }

  for (int round = 0; round < kTestRounds; ++round) {
    int64_t a = kTestBase[round];

    // Fermet primality test.
    int64_t x = Power(a, d, n);
    if (x == 1 || x == n - 1)
      continue;

    // Witness primality test.
    for (int i = 0; i < r - 1; ++i) {
      x = Multiply(x, x, n);
      if (x == n - 1)
        break;
    }
    if (x != n - 1)
      return false;
  }

  return true;
}

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_PRIME_MILLER_RABIN_H