#ifndef HIRA_MATH_MODULAR_FACTORIAL_H
#define HIRA_MATH_MODULAR_FACTORIAL_H

#include "hira/common_header.h"

namespace hira {

namespace math {

template <typename ModInt>
ModInt fact(int n) {
  static std::vector<ModInt> f({1});

  int m = f.size();
  if (m < n + 1) {
    f.resize(n + 1);

    for (int i = m; i <= n; ++i)
      f[i] = f[i - 1] * i;
  }

  return f[n];
}

template <typename ModInt>
ModInt inv_fact(int n) {
  static std::vector<ModInt> g({1});

  int m = g.size();
  if (m < n + 1) {
    g.resize(n + 1);

    g[n] = fact<ModInt>(n).inv();
    for (int i = n - 1; i >= m; --i)
      g[i] = g[i + 1] * (i + 1);
  }

  return g[n];
}

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_MODULAR_FACTORIAL_H