#ifndef HIRA_STD_EXTENSION_HASH_CONTAINER_H
#define HIRA_STD_EXTENSION_HASH_CONTAINER_H

#include "hira/common_header.h"

namespace hira {

namespace stdext {

struct HashWithRandom {
  static uint64_t FIXED_RANDOM;

  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  template <typename T>
  uint64_t operator()(const T x) const {
    return splitmix64(x + FIXED_RANDOM);
  }

  template <typename First, typename Second>
  uint64_t operator()(const std::pair<First, Second>& x) const {
    return splitmix64(x.first + FIXED_RANDOM) ^
           (splitmix64(x.second + FIXED_RANDOM) >> 1);
  }
};
uint64_t HashWithRandom::FIXED_RANDOM =
    std::chrono::steady_clock::now().time_since_epoch().count();

template <typename Key, typename Value>
using hashmap = std::unordered_map<Key, Value, HashWithRandom>;

template <typename Value>
using hashset = std::unordered_set<Value, HashWithRandom>;

}  // namespace stdext

}  // namespace hira

#endif  // HIRA_STD_EXTENSION_HASH_CONTAINER_H