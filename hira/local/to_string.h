#ifndef HIRA_LOCAL_TO_STRING_H
#define HIRA_LOCAL_TO_STRING_H

#include "hira/common_header.h"

#ifdef LOCAL

#include "hira/local/type_name.h"

template <typename T>
std::string serialize(T t);

namespace my {

std::string to_string(const std::string& s) {
  return '"' + s + '"';
}

std::string to_string(__int128_t x) {
  if (x == 0)
    return "0";

  std::string s;
  bool is_negative = false;
  if (x < 0) {
    is_negative = true;
    x = -x;
  }
  while (x) {
    s.push_back('0' + x % 10);
    x /= 10;
  }
  std::reverse(s.begin(), s.end());
  if (is_negative)
    s = "-" + s;

  return s;
}

template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p) {
  return "(" + serialize(p.first) + "," + serialize(p.second) + ")";
}

template <size_t N>
std::string to_string(const std::bitset<N>& v) {
  std::string res = "";
  for (size_t i = 0; i < N; i++)
    res += static_cast<char>('0' + v[i]);
  return res;
}

template <typename StreamType, int index, typename TupleType, int size>
void tuple_to_string(StreamType& stream, TupleType tuple) {
  stream << serialize(std::get<index>(tuple)) << ",";
  constexpr int next_index = index + 1;
  if constexpr (next_index < size)
    tuple_to_string<StreamType, next_index, TupleType, size>(stream, tuple);
}

template <typename... Args>
std::string to_string(const std::tuple<Args...>& tuple) {
  std::stringstream ss;
  ss << "(";
  tuple_to_string<std::stringstream, 0, decltype(tuple), sizeof...(Args)>(
      ss, tuple);
  ss << ")";
  return ss.str();
}

template <typename T, typename = void>
struct iterable : std::false_type {};

template <typename T>
struct iterable<T, std::void_t<typename T::iterator>> : std::true_type {};

template <typename T, typename = std::enable_if_t<iterable<T>::value>>
struct child_type_helper {
  using type = typename T::value_type;
};

template <typename T>
using child_type = typename child_type_helper<T>::type;

template <typename T, typename = void>
struct child_iterable : std::false_type {};

template <typename T>
struct child_iterable<T, std::void_t<typename child_type<T>::iterator>>
    : std::true_type {};

static int container_recurse_level = 0;
const static int TAB_WIDTH = 4;
template <typename Container,
          typename = std::enable_if_t<iterable<Container>::value>>
std::string to_string(const Container& container) {
  std::stringstream ss;

  if (container_recurse_level == 0)
    ss << get_type_name<Container>();

  ss << "[";
  if (child_iterable<Container>::value)
    ss << "\n";

  ++container_recurse_level;
  for (auto it = container.begin(); it != container.end(); ++it) {
    if (child_iterable<Container>::value)
      ss << std::string(container_recurse_level * TAB_WIDTH, ' ');

    ss << serialize(*it);
    if (child_iterable<Container>::value)
      ss << "\n";
    else
      ss << ", ";
  }
  --container_recurse_level;

  if (child_iterable<Container>::value)
    ss << std::string(container_recurse_level * TAB_WIDTH, ' ');
  ss << "]";

  return ss.str();
}

}  // namespace my

template <typename T, typename = std::string>
struct has_std_to_string : std::false_type {};

template <typename T>
struct has_std_to_string<T, decltype(std::to_string(std::declval<T>()))>
    : std::true_type {};

template <typename T, typename = std::string>
struct has_my_to_string : std::false_type {};

template <typename T>
struct has_my_to_string<T, decltype(my::to_string(std::declval<T>()))>
    : std::true_type {};

template <typename T, typename = std::string>
struct has_self_to_string : std::false_type {};

template <typename T>
struct has_self_to_string<T, decltype(std::declval<T>().to_string())>
    : std::true_type {};

template <typename T>
std::string serialize(T t) {
  if constexpr (has_my_to_string<T>::value) {
    return my::to_string(t);
  }
  if constexpr (has_self_to_string<T>::value) {
    return t.to_string();
  }
  if constexpr (has_std_to_string<T>::value) {
    return std::to_string(t);
  }
  return "no matched to_string";
}

#else

#define serialize(...)

#endif  // LOCAL

#endif  // HIRA_LOCAL_TO_STRING_H
