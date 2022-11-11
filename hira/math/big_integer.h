#ifndef HIRA_MATH_BIG_INTEGER_H
#define HIRA_MATH_BIG_INTEGER_H

#include "hira/common_header.h"

#include "hira/math/polynomial/ntt.h"

namespace hira {

namespace math {

/**
 * Implementation of 10-based big integer, with about O(n log
 * n) multiplication and division.
 *
 * Idea
 *
 *   O(n \log n) multiplication: FFT or NTT.
 *   O(n \log n) division: Newton-Raphson Division.
 *
 * Reference
 *
 *   1. https://www.luogu.com.cn/blog/242973/solution-p5432 by
 * hly1024
 *   2.
 * https://en.wikipedia.org/wiki/Division_algorithm#Fast_division_methods
 */
class BigInteger {
 public:
  bool IsZero() const { return sign_ == 1 && a_.size() == 1 && a_[0] == 0; }

  int CountDigit() const { return a_.size(); }

  void Normalize() {
    int x = 0;
    for (int i = 0; i < a_.size(); ++i) {
      x += a_[i];
      a_[i] = x % 10;
      x /= 10;
    }

    while (x) {
      a_.push_back(x % 10);
      x /= 10;
    }
  }

  void Shrink() {
    while (a_.size() > 1 && a_.back() == 0)
      a_.pop_back();
    if (a_.size() == 1 && a_[0] == 0)
      sign_ = 1;
  }

 public:
  BigInteger& operator+=(const BigInteger& other) {
    if (sign_ != other.sign_)
      return (*this) = (*this) - (-other);

    int n = CountDigit(), m = other.CountDigit();
    if (n < m)
      a_.resize(m);

    int carry = 0;
    for (int i = 0; i < n; ++i) {
      int x = a_[i] + (i < m ? other.a_[i] : 0) + carry;
      a_[i] = x >= 10 ? x - 10 : x;
      carry = x >= 10 ? 1 : 0;
    }
    if (carry > 0)
      a_.push_back(1);

    return (*this);
  }

  BigInteger& operator-=(const BigInteger& other) {
    if (sign_ != other.sign_)
      return (*this) = (*this) + (-other);

    if (abs() < other.abs())
      return (*this) = -(other - (*this));

    int n = CountDigit(), m = other.CountDigit();
    int borrow = 0;
    for (int i = 0; i < n; ++i) {
      int x = a_[i] - (i < m ? other.a_[i] : 0) - borrow;
      a_[i] = x >= 0 ? x : 10 + x;
      borrow = x >= 0 ? 0 : 1;
    }

    Shrink();

    return (*this);
  }

  BigInteger& operator*=(const BigInteger& other) {
    if (IsZero())
      return (*this);

    if (other.IsZero()) {
      (*this) = BigInteger();
      return (*this);
    }

    sign_ = sign_ * other.sign_;
    a_ = NTT::convolve(a_, other.a_);

    Normalize();
    Shrink();

    return (*this);
  }

  BigInteger& operator/=(const BigInteger& other) {
    if (IsZero())
      return (*this);

    if (abs() < other.abs()) {
      (*this) = BigInteger();
      return (*this);
    }

    int new_sign = sign_ * other.sign_;

    std::function<BigInteger(BigInteger)> compute =
        [&](const BigInteger& a) -> BigInteger {
      int n = a.CountDigit();

      if (n == 1)
        return BigInteger(100 / a.a_[0]);

      if (n == 2)
        return BigInteger((10000 / (a.a_[0] + a.a_[1] * 10)));

      int np = n / 2 + 1;
      BigInteger q = compute(a >> (n - np)) << (n - np);
      return (q * ((BigInteger(2) << (2 * n)) - q * a)) >> (2 * n);
    };

    std::function<BigInteger(BigInteger)> compute_accurate =
        [&](const BigInteger& a) -> BigInteger {
      std::array<BigInteger, 7> t;
      t[0] = a;
      for (int i = 1; i < 7; ++i)
        t[i] = t[i - 1] + t[i - 1];

      int n = a.CountDigit(), error = 0;
      BigInteger r = compute(a);
      BigInteger diff = (BigInteger(1) << (2 * n)) - a * r;

      for (int i = 6; i >= 0; --i) {
        if (diff >= t[i]) {
          diff -= t[i];
          error |= 1 << i;
        }
      }
      r += error;

      return r;
    };

    int n = other.CountDigit(), m = CountDigit();
    int offset = m <= 2 * n ? 2 * n : n + m;

    BigInteger w = other.abs();
    if (m > 2 * n)
      w <<= (m - n);

    BigInteger x = compute_accurate(w);
    BigInteger y = (abs() * x) >> offset;
    BigInteger z = abs() - y * other.abs();
    if (z < other.abs())
      (*this) = y;
    else
      (*this) = y + BigInteger(1);
    sign_ = new_sign;

    return (*this);
  }

  BigInteger& operator%=(const BigInteger& other) {
    (*this) = (*this) - (*this) / other * other;
    return (*this);
  }

  BigInteger& operator<<=(int x) {
    if (IsZero())
      return (*this);

    a_.insert(a_.begin(), x, 0);
    return (*this);
  }

  BigInteger& operator>>=(int x) {
    if (IsZero())
      return (*this);

    if (x >= a_.size()) {
      (*this) = 0;
      sign_ = 1;
      return (*this);
    }

    a_.erase(a_.begin(), a_.begin() + x);
    return (*this);
  }

  BigInteger operator-() const {
    if (IsZero())
      return BigInteger();

    BigInteger result(*this);
    result.sign_ *= -1;
    return result;
  }

  BigInteger operator+(const BigInteger& other) const {
    BigInteger result = (*this);
    result += other;
    return result;
  }

  BigInteger operator-(const BigInteger& other) const {
    BigInteger result = (*this);
    result -= other;
    return result;
  }

  BigInteger operator*(const BigInteger& other) const {
    BigInteger result = (*this);
    result *= other;
    return result;
  }

  BigInteger operator/(const BigInteger& other) const {
    BigInteger result = (*this);
    result /= other;
    return result;
  }

  BigInteger operator%(const BigInteger& other) const {
    BigInteger result = (*this);
    result %= other;
    return result;
  }

  BigInteger operator<<(int x) const {
    BigInteger result = (*this);
    result <<= x;
    return result;
  }

  BigInteger operator>>(int x) const {
    BigInteger result = (*this);
    result >>= x;
    return result;
  }

  BigInteger abs() const {
    BigInteger result = (*this);
    result.sign_ = 1;
    return result;
  }

 public:
  bool operator==(const BigInteger& other) const {
    return sign_ == other.sign_ && a_.size() == other.a_.size() &&
           a_ == other.a_;
  }

  bool operator<(const BigInteger& other) const {
    if (sign_ != other.sign_)
      return sign_ < other.sign_;

    if (a_.size() != other.a_.size())
      return a_.size() < other.a_.size();

    for (int i = a_.size() - 1; i >= 0; --i) {
      if (a_[i] != other.a_[i])
        return a_[i] < other.a_[i];
    }

    return false;
  }

  bool operator>(const BigInteger& other) const {
    if (sign_ != other.sign_)
      return sign_ > other.sign_;

    if (a_.size() != other.a_.size())
      return a_.size() > other.a_.size();

    for (int i = a_.size() - 1; i >= 0; --i) {
      if (a_[i] != other.a_[i])
        return a_[i] > other.a_[i];
    }

    return false;
  }

  bool operator!=(const BigInteger& other) const { return !((*this) == other); }

  bool operator<=(const BigInteger& other) const { return !((*this) > other); }

  bool operator>=(const BigInteger& other) const { return !((*this) < other); }

  BigInteger& operator=(const BigInteger&) = default;

 public:
  std::string to_string() const {
    std::string s;
    if (sign_ == -1)
      s.push_back('-');

    for (int i = a_.size() - 1; i >= 0; --i)
      s.push_back(a_[i] + '0');
    return s;
  }

  friend std::istream& operator>>(std::istream& is, BigInteger& rhs) {
    std::string x;
    is >> x;
    rhs = BigInteger(x);
    return is;
  }
  friend std::ostream& operator<<(std::ostream& os, const BigInteger& rhs) {
    return os << rhs.to_string();
  }

 public:
  BigInteger() : a_({0}), sign_(1) {}

  template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
  BigInteger(T value) {
    if (value == 0) {
      sign_ = 1;
      a_.push_back(0);
      return;
    }

    if (value < 0) {
      sign_ = -1;
      value = -value;
    } else {
      sign_ = 1;
    }

    while (value) {
      a_.push_back(value % 10);
      value /= 10;
    }
  }

  BigInteger(const std::string& s) {
    if (s.empty()) {
      sign_ = 1;
      a_ = {0};
      return;
    }

    int start = 0;
    if (s[0] == '+') {
      sign_ = 1;
      start = 1;
    } else if (s[0] == '-') {
      sign_ = -1;
      start = 1;
    } else {
      sign_ = 1;
    }

    a_.reserve(s.size() - start);
    for (int i = s.size() - 1; i >= start; --i)
      a_.push_back(s[i] - '0');
  }

  BigInteger(BigInteger&& other) {
    sign_ = other.sign_;
    a_ = std::move(other.a_);
    other.a_ = {};
  }

  BigInteger(const BigInteger&) = default;

  ~BigInteger() = default;

 private:
  std::vector<int> a_;
  int sign_;
};

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_BIG_INTEGER_H