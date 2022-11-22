#ifndef HIRA_STD_EXTENSION_HEAP_H
#define HIRA_STD_EXTENSION_HEAP_H

#include "hira/common_header.h"

namespace hira {

namespace stdext {

template <typename T>
using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T>
using max_heap = std::priority_queue<T, std::vector<T>, std::less<T>>;

}  // namespace stdext

}  // namespace hira

#endif  // HIRA_STD_EXTENSION_HEAP_H