#ifndef HIRA_MATH_COMPLEX_H
#define HIRA_MATH_COMPLEX_H

#include "hira/common_header.h"

namespace hira {

namespace math {

template <typename Double>
class Complex {
 public:
  Complex operator-(const Complex& c) const {
    return Complex(real_ - c.real_, imagine_ - c.imagine_);
  }

  Complex operator+(const Complex& c) const {
    return Complex(real_ + c.real_, imagine_ + c.imagine_);
  }

  Complex operator*(const Complex& c) const {
    return Complex(real_ * c.real_ - imagine_ * c.imagine_,
                   real_ * c.imagine_ + imagine_ * c.real_);
  }

  Complex operator/(Double d) const { return Complex(real_ / d, imagine_ / d); }

 public:
  Complex() : real_(0), imagine_(0) {}

  Complex(Double real, Double imagine) : real_(real), imagine_(imagine) {}

  Complex conjugation() const { return Complex(real_, -imagine_); }

  Double real() const { return real_; }

  Double imagine() const { return imagine_; }

 private:
  Double real_;
  Double imagine_;
};

}  // namespace math

}  // namespace hira

#endif  // HIRA_MATH_COMPLEX_H