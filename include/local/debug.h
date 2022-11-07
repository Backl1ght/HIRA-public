#ifndef LOLCPC_INCLUDE_LOCAL_DEBUG_H
#define LOLCPC_INCLUDE_LOCAL_DEBUG_H

#ifdef LOCAL

#include "to_string.h"

template <typename T>
inline void logd_impl(const char* format, T value) {
  std::cerr << format << '=' << serialize(value) << std::endl;
}

template <typename First, typename... Rest>
inline void logd_impl(const char* format, First f, Rest... r) {
  while (*format != ',')
    std::cerr << *format++;
  std::cerr << '=' << serialize(f) << ",";
  logd_impl(format + 1, r...);
}

#define logd(...)                \
  std::cerr << __LINE__ << ": "; \
  logd_impl(#__VA_ARGS__, __VA_ARGS__);

#else

#define logd(...)

#endif  // LOCAL

#endif  // LOLCPC_INCLUDE_LOCAL_DEBUG_H
