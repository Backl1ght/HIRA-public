#ifndef HIRA_MATH_MODULAR_MOD_INT_H
#define HIRA_MATH_MODULAR_MOD_INT_H

#include "hira/common_header.h"

namespace hira {

namespace math {

template <typename ValueType, int id, typename SupperType>
class ModInt {
  static ValueType normalize(ValueType value) {
    if (value >= 0 && value < mod_)
      return value;
    value %= mod_;
    if (value < 0)
      value += mod_;
    return value;
  }

  template <typename ExponentType>
  static ValueType power(ValueType value, ExponentType exponent) {
    ValueType result = 1;
    ValueType base = value;
    while (exponent) {
      if (exponent & 1)
        result = SupperType(result) * base % mod_;
      base = SupperType(base) * base % mod_;
      exponent >>= 1;
    }
    return result;
  }

 public:
  ModInt() : value_(0) {}

  ModInt(ValueType value) : value_(normalize(value)) {}

  ModInt(SupperType value) : value_(normalize(value % mod_)) {}

  ValueType value() const { return value_; }

  ModInt inv() const { return ModInt(power(value_, mod_ - 2)); }

  template <typename ExponentType>
  ModInt power(ExponentType exponent) const {
    return ModInt(power(value_, exponent));
  }

  ModInt operator-() const { return ModInt(mod_ - value_); }

  ModInt& operator+=(const ModInt& other) {
    value_ = value_ + other.value() >= mod_ ? value_ + other.value() - mod_
                                            : value_ + other.value();
    return (*this);
  }

  ModInt& operator-=(const ModInt& other) {
    value_ = value_ - other.value() < 0 ? value_ - other.value() + mod_
                                        : value_ - other.value();
    return (*this);
  }

  ModInt& operator*=(const ModInt& other) {
    value_ = SupperType(1) * value_ * other.value() % mod_;
    return (*this);
  }

  ModInt& operator/=(const ModInt& other) {
    value_ = SupperType(1) * value_ * other.inv().value() % mod_;
    return (*this);
  }

  ModInt operator+(const ModInt& other) const {
    ModInt result = *this;
    result += other;
    return result;
  }

  ModInt operator-(const ModInt& other) const {
    ModInt result = *this;
    result -= other;
    return result;
  }

  ModInt operator*(const ModInt& other) const {
    ModInt result = *this;
    result *= other;
    return result;
  }

  ModInt operator/(const ModInt& other) const {
    ModInt result = *this;
    result /= other;
    return result;
  }

  std::string to_string() const { return std::to_string(value_); }

  static void set_mod(ValueType mod) { mod_ = mod; }

  static ValueType get_mod() { return mod_; }

 private:
  ValueType value_;
  static ValueType mod_;
};

template <typename ValueType, int id, typename SupperType>
ValueType ModInt<ValueType, id, SupperType>::mod_ = -1;

using ModInt107 = ModInt<i32, 1'000'000'007, i64>;
template <>
i32 ModInt107::mod_ = 1'000'000'007;

using ModInt109 = ModInt<i32, 1'000'000'009, i64>;
template <>
i32 ModInt109::mod_ = 1'000'000'009;

using ModInt998 = ModInt<i32, 998'244'353, i64>;
template <>
i32 ModInt998::mod_ = 998'244'353;

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_MODULAR_MOD_INT_H