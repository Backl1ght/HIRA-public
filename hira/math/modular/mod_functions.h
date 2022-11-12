#ifndef HIRA_MATH_MODULAR_MOD_FUNCTINOS_H
#define HIRA_MATH_MODULAR_MOD_FUNCTINOS_H

#include "hira/common_header.h"

#include "hira/misc/i128.h"

namespace hira {

namespace math {

/**
 * @TODO(backlight): __int128_t is ugly, try optimize it.
 */

inline int64_t Add(int64_t a, int64_t b, int64_t mod) {
  return ((__int128_t)a + b) % mod;
}

inline int64_t Substract(int64_t a, int64_t b, int64_t mod) {
  return (((__int128_t)a - b) % mod + mod) % mod;
}

inline int64_t Multiply(int64_t a, int64_t b, int64_t mod) {
  return (__int128_t)a * b % mod;
}

inline int64_t Power(int64_t a, int64_t b, int64_t mod) {
  int64_t r = 1;
  while (b) {
    if (b & 1)
      r = Multiply(r, a, mod);
    a = Multiply(a, a, mod);
    b >>= 1;
  }
  return r;
}

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_MODULAR_MOD_FUNCTINOS_H