#ifndef HIRA_LOCAL_TYPE_NAME_H
#define HIRA_LOCAL_TYPE_NAME_H

#include "hira/common_header.h"

#ifdef LOCAL

// code from https://stackoverflow.com/a/66551751
template <typename T>
constexpr auto full_function_name() -> std::string_view {
#if defined(__clang__) || defined(__GNUC__)
  return std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
  return std::string_view{__FUNCSIG__};
#else
#error Unsupported compiler
#endif
}

// Outside of the template so its computed once
struct type_name_info {
  static constexpr auto sentinel_function = full_function_name<double>();
  static constexpr auto prefix_offset = sentinel_function.find("double");
  static constexpr auto suffix_offset =
      sentinel_function.size() - prefix_offset - /* strlen("double") */ 6;
};

template <typename T>
constexpr auto get_type_name() -> std::string_view {
  constexpr auto function = full_function_name<T>();

  const auto start = type_name_info::prefix_offset;
  const auto end = function.size() - type_name_info::suffix_offset;
  const auto size = end - start;

  return function.substr(start, size);
}

#endif  // LOCAL

#endif  // HIRA_LOCAL_TYPE_NAME_H