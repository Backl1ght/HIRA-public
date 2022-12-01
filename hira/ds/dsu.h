#ifndef HIRA_DS_DSU_H
#define HIRA_DS_DSU_H

#include "hira/common_header.h"

namespace hira {

namespace ds {

class DSU {
 public:
  DSU(int n) : n_(n), f_(n_) {
    for (int i = 0; i < n_; ++i) {
      f_[i] = i;
    }
  }

  int leader(int x) {
    if (x != f_[x])
      f_[x] = leader(f_[x]);

    return f_[x];
  }

  bool merge(int x, int y) {
    x = leader(x), y = leader(y);
    if (x == y)
      return false;

    f_[y] = x;
    return true;
  }

 private:
  int n_;
  std::vector<int> f_;
};

}  // namespace ds

}  // namespace hira

#endif  // HIRA_DS_DSU_H