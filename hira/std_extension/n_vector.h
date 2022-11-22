#ifndef HIRA_STD_EXTENSION_N_VECTOR_H
#define HIRA_STD_EXTENSION_N_VECTOR_H

#include "hira/common_header.h"

namespace hira {

namespace stdext {

template <typename T, typename... Args>
auto n_vector(size_t n, Args&&... args) {
  if constexpr (sizeof...(args) == 1) {
    return std::vector<T>(n, args...);
  } else {
    return std::vector(n, n_vector<T>(args...));
  }
}

}  // namespace stdext

}  // namespace hira

#endif  // HIRA_STD_EXTENSION_N_VECTOR_H