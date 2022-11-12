#ifndef HIRA_MISC_I128_H
#define HIRA_MISC_I128_H

#include "hira/common_header.h"

namespace hira {

using i128 = __int128_t;
using u128 = __uint128_t;

i128 stoi(const std::string& s, int base) {
  int start = 0;
  if (s[0] == '-')
    start = 1;

  i128 x = 0;
  for (int i = start; i < s.size(); ++i) {
    x = x * base + (s[i] - '0');
  }

  if (start != 0)
    x = -x;

  return x;
}

std::string itos(i128 x) {
  if (x == 0)
    return "0";

  std::string s;
  while (x) {
    s.push_back(x % 10 + '0');
    x /= 10;
  }
  std::reverse(s.begin(), s.end());

  return s;
}

std::ostream& operator<<(std::ostream& os, i128& x) {
  return os << itos(x);
}

std::istream& operator>>(std::istream& is, i128& x) {
  std::string s;
  is >> s;
  x = stoi(s);
  return is;
}

}  // namespace hira

#endif  // HIRA_MISC_I128_H
