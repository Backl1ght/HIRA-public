#ifndef HIRA_LOCAL_DEBUG_H
#define HIRA_LOCAL_DEBUG_H

#include "hira/common_header.h"

#ifdef LOCAL

#include "terminal_color_modifier.h"
#include "to_string.h"

template <typename T>
inline void logd_impl(const char* format, T value) {
  std::cerr << format << '=' << serialize(value) << std::endl;
}

template <typename First, typename... Rest>
inline void logd_impl(const char* format, First f, Rest... r) {
  i32 level = 0;
  while (*format != ',' || level != 0) {
    std::cerr << *format++;
    if (*format == '(' || *format == '{' || *format == '<' || *format == '[')
      ++level;
    if (*format == ')' || *format == '}' || *format == '>' || *format == ']')
      --level;
  }
  std::cerr << '=' << serialize(f) << ",";
  logd_impl(format + 1, r...);
}

#define logd(...)                             \
  do {                                        \
    std::cerr << foreground_red_modifier;     \
    std::cerr << __LINE__ << ": ";            \
    logd_impl(#__VA_ARGS__, __VA_ARGS__);     \
    std::cerr << foreground_default_modifier; \
  } while (false);

#else

#define logd(...)

#endif  // LOCAL

#endif  // HIRA_LOCAL_DEBUG_H
