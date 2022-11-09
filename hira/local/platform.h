#ifndef HIRA_LOCAL_PLATFORM_H
#define HIRA_LOCAL_PLATFORM_H

#include "hira/common_header.h"

#ifdef LOCAL

#if defined(__linux__)

namespace LocalStackLimit {

#include <sys/resource.h>

class StackLimitHelper {
 public:
  StackLimitHelper(int limit) {
    // set min stack size to $limit MB
    const rlim_t kStackSize = limit * 1024 * 1024;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
      if (rl.rlim_cur < kStackSize) {
        rl.rlim_cur = kStackSize;
        result = setrlimit(RLIMIT_STACK, &rl);
        if (result != 0) {
          fprintf(stderr, "setrlimit returned result = %d\n", result);
        }
      }
    }
    fprintf(stderr, "set min stack size to %lu MB\n", kStackSize / 1024 / 1024);
  }
} _(256);

}  // namespace LocalStackLimit

#elif defined(_WIN32)

#else

#error Not supported platform!

#endif

#endif  // LOCAL

#endif  // HIRA_LOCAL_PLATFORM_H
