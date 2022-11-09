#ifndef HIRA_MISC_IO_H
#define HIRA_MISC_IO_H

#include "hira/common_header.h"

namespace hira {

#ifdef FASTIO

namespace hira {

class FastIO {
  // TODO(backlight): finish the code.
} io;

#endif

#define STDIO                       \
  std::ios::sync_with_stdio(false); \
  std::cin.tie(0);                  \
  std::cout.tie(0);

}  // namespace hira

#endif  // HIRA_MISC_IO_H