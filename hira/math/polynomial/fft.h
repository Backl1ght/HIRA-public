#ifndef HIRA_MATH_POLYNOMIAL_FFT_H
#define HIRA_MATH_POLYNOMIAL_FFT_H

#include "hira/common_header.h"
#include "hira/math/bit_reversal_transform.h"
#include "hira/math/complex.h"
#include "hira/math/constant.h"

namespace hira {

namespace math {

template <typename Double>
class FastFourierTransform {
 public:
  using complex_t = Complex<Double>;

 private:
  static int GetProperLength(int n) {
    int l = 1;
    while (l < n)
      l <<= 1;
    return l;
  }

  static void dft(std::vector<complex_t>& a) {
    int n = a.size();
    BitReversalTransform(a);

    for (int i = 2; i <= n; i <<= 1) {
      int l = i >> 1;
      complex_t w, x, wn(std::cos(pi / l), std::sin(pi / l));
      for (int j = 0; j < n; j += i) {
        w = complex_t(1, 0);
        for (int k = j; k < j + l; ++k) {
          x = a[k + l] * w;
          a[k + l] = a[k] - x;
          a[k] = a[k] + x;
          w = w * wn;
        }
      }
    }
  }

  static void idft(std::vector<complex_t>& a) {
    dft(a);
    std::reverse(a.begin() + 1, a.end());
    int n = a.size();
    for (int i = 0; i < a.size(); ++i)
      a[i] = a[i] / n;
  }

 public:
  template <typename Integer>
  static std::vector<Integer> convolve(const std::vector<Integer>& f,
                                       const std::vector<Integer>& g) {
    int n = f.size(), m = g.size(), k = n + m - 1;
    int l = GetProperLength(k);

    std::vector<complex_t> a(l), b(l);
    for (int i = 0; i < n; ++i)
      a[i] = complex_t(f[i], 0);
    for (int i = 0; i < m; ++i)
      b[i] = complex_t(g[i], 0);

    dft(a);
    dft(b);
    for (int i = 0; i < l; ++i)
      a[i] = a[i] * b[i];
    idft(a);

    std::vector<Integer> h(k);
    for (int i = 0; i < k; ++i)
      h[i] = Integer(a[i].real() + 0.5);

    return h;
  }

  // TODO(backlight): add convolve mod x.
};

template <typename Double>
using FFT = FastFourierTransform<Double>;

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_POLYNOMIAL_FFT_H