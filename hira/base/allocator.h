#ifndef HIRA_DS_ALLOCATOR_H
#define HIRA_DS_ALLOCATOR_H

#include "hira/common_header.h"

namespace hira {

namespace base {

template <typename T>
class InstantAllocator {
 public:
  using value_type = T;
  using pointer = value_type*;

 public:
  pointer Allocate() { return new value_type; }

  void Free(pointer& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  void FreeDtor(pointer& p) { Free(p); }
};

template <typename T, size_t N, bool enable_cache = true>
class PooledAllocator {
 public:
  using value_type = T;
  using pointer = typename std::array<T, N>::pointer;

 private:
  std::array<T, N> pool_;
  std::queue<pointer> cache_;
  pointer curr_;

 public:
  PooledAllocator() : pool_(), cache_(), curr_(pool_.begin()) {}

  pointer Allocate() {
    if constexpr (enable_cache) {
      if (!cache_.empty()) {
        pointer ret = cache_.front();
        cache_.pop();

        return ret;
      }
    }

    return curr_++;
  }

  void Free(pointer& p) {
    if (p) {
      if constexpr (enable_cache) {
        cache_.push(p);
      }

      p = nullptr;
    }
  }

  void FreeDtor(pointer& p) {}
};

}  // namespace base
}  // namespace hira

#endif  // HIRA_DS_ALLOCATOR_H