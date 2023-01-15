#ifndef HIRA_MATH_PRIME_MILLER_RABIN_H
#define HIRA_MATH_PRIME_MILLER_RABIN_H

#include "hira/common_header.h"

#include "hira/math/modular/mod_functions.h"
#include "hira/misc/i128.h"

namespace hira {

namespace math {

// Time Complexity: $O(k \log^{3} n)$
bool MillerRabinTest(int64_t n) {
  // reference: http://miller-rabin.appspot.com/
  constexpr static int kTestRounds32 = 3;
  constexpr static int kTestBase32[kTestRounds32] = {2, 7, 61};
  constexpr static int kTestRounds64 = 7;
  constexpr static int kTestBase64[kTestRounds64] = {
      2, 325, 9375, 28178, 450775, 9780504, 1795265022};

  if (n < 2)
    return false;

  if (n == 2 || n == 7 || n == 61)
    return true;

  if (n % 2 == 0)
    return false;

  int test_rounds;
  const int* test_base;
  if (n <= std::numeric_limits<uint32_t>::max()) {
    test_rounds = kTestRounds32;
    test_base = kTestBase32;
  } else {
    test_rounds = kTestRounds64;
    test_base = kTestBase64;
  }

  int64_t d = n - 1, r = 0;
  while (d % 2 == 0) {
    d >>= 1;
    r = r + 1;
  }

  for (int round = 0; round < test_rounds; ++round) {
    int64_t a = test_base[round];

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