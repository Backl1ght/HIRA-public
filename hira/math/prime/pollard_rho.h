#ifndef HIRA_MATH_PRIME_POLLARD_RHO_H
#define HIRA_MATH_PRIME_POLLARD_RHO_H

#include "hira/common_header.h"

#include "hira/math/prime/miller_rabin.h"

namespace hira {

namespace math {

static std::mt19937_64 rng_(
    std::chrono::steady_clock::now().time_since_epoch().count());

int64_t Rand(int64_t l, int64_t r) {
  return l + rng_() % (r - l + 1);
}

int64_t Rho(int64_t n) {
  // Can not factor 4 because the faster step gap is 2.
  if (n == 4)
    return 2;

  const static int kMaxStepSize = 1 << 7;

  int64_t c;
  std::function<int64_t(int64_t)> f = [&n, &c](int64_t x) {
    return Add(Multiply(x, x, n), c, n);
  };

  // Since n is not a prime, there must be a non-trivial factor of n.
  for (;;) {
    /**
     * Brent's cycle finding method, and replace k gcd steps with k - 1
     * multiplications modulo n and a single gcd. reference:
     * https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm#Variants.
     */
    c = Rand(3, n - 1);

    int64_t x = Rand(0, n - 1);
    int64_t t = f(x), r = f(t);
    int64_t goal = 1, curr = 1;
    int64_t d1, d2 = 1;
    while (t != r) {
      d1 = Multiply(d2, std::abs(r - t), n);
      if (d1 == 0) {
        break;
      } else {
        d2 = d1;
      }

      if (curr % kMaxStepSize == 0) {
        int64_t d = std::gcd(d2, n);
        if (d != 1 && d != n)
          return d;
      }

      if (curr == goal) {
        int64_t d = std::gcd(d2, n);
        if (d != 1 && d != n)
          return d;

        t = r;
        goal = goal * 2;
        curr = 0;
      }

      r = f(r);
      ++curr;
    }
  }
}

std::vector<int64_t> Factor(int64_t n) {
  std::vector<int64_t> factors;
  std::function<void(int64_t)> factor = [&](int64_t n) {
    if (n < 2)
      return;

    if (MillerRabinTest(n)) {
      factors.push_back(n);
    } else {
      int64_t x = Rho(n);
      while (n % x == 0)
        n /= x;
      factor(x);
      factor(n);
    }
  };
  factor(n);
  std::sort(factors.begin(), factors.end());
  return factors;
}

std::vector<std::pair<int64_t, int64_t>> FactorPE(int64_t n) {
  auto p = Factor(n);

  std::vector<std::pair<int64_t, int64_t>> pe(p.size());
  for (int i = 0; i < n; ++i) {
    pe[i].first = p[i];
    while (n % p[i] == 0) {
      n /= p[i];
      ++pe[i].second;
    }
  }

  return pe;
}

std::vector<int64_t> FactorAll(int64_t n) {
  auto pe = FactorPE(n);

  int size = 1;
  for (int i = 0; i < pe.size(); ++i)
    size *= (pe[i].second + 1);

  std::vector<int64_t> all;
  all.reserve(size);

  std::function<void(int, int64_t)> dfs = [&](int p, int64_t x) {
    if (p == pe.size()) {
      all.push_back(x);
      return;
    }

    for (int i = 0; i <= pe[p].second; ++i)
      dfs(p + 1, x * pe[i].first);
  };
  dfs(0, 1);

  return all;
}

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_PRIME_POLLARD_RHO_H