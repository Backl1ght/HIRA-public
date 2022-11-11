#ifndef HIRA_MATH_POLYNOMIAL_NTT_H
#define HIRA_MATH_POLYNOMIAL_NTT_H

#include "hira/common_header.h"

#include "hira/math/bit_reversal_transform.h"

namespace hira {

namespace math {

// TODO(backlight): make this more universal.
class FastNumberTheoreticTransform {
 private:
  const static int P = 998244353, G = 3;
  static std::vector<int> roots;

  static int power(int a, int b) {
    int r = 1;
    while (b) {
      if (b & 1)
        r = 1ll * r * a % P;
      a = 1ll * a * a % P;
      b >>= 1;
    }
    return r;
  }

 public:
  static void dft(std::vector<int>& a) {
    int n = a.size();
    BitReversalTransform(a);

    if (int(roots.size()) < n) {
      int k = __builtin_ctz(roots.size());
      roots.resize(n);
      while ((1 << k) < n) {
        int e = power(G, (P - 1) >> (k + 1));
        for (int i = 1 << (k - 1); i < (1 << k); ++i) {
          roots[2 * i] = roots[i];
          roots[2 * i + 1] = 1ll * roots[i] * e % P;
        }
        ++k;
      }
    }

    for (int k = 1; k < n; k *= 2) {
      for (int i = 0; i < n; i += 2 * k) {
        for (int j = 0; j < k; ++j) {
          int u = a[i + j];
          int v = 1ll * a[i + j + k] * roots[k + j] % P;
          int x = u + v;
          if (x >= P)
            x -= P;
          a[i + j] = x;
          x = u - v;
          if (x < 0)
            x += P;
          a[i + j + k] = x;
        }
      }
    }
  }

  static void idft(std::vector<int>& a) {
    int n = a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    int inv = power(n, P - 2);
    for (int i = 0; i < n; ++i)
      a[i] = 1ll * a[i] * inv % P;
  }

  static std::vector<int> convolve(const std::vector<int>& a,
                                   const std::vector<int>& b) {
    int sz = 1, k = a.size() + b.size() - 1;
    while (sz < k)
      sz <<= 1;
    std::vector<int> p = a, q = b;
    p.resize(sz);
    q.resize(sz);

    dft(p);
    dft(q);
    for (int i = 0; i < sz; ++i)
      p[i] = 1ll * p[i] * q[i] % P;
    idft(p);

    return p;
  }
};

std::vector<int> FastNumberTheoreticTransform::roots{0, 1};

using NTT = FastNumberTheoreticTransform;

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_POLYNOMIAL_NTT_H