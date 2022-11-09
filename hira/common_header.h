#ifndef HIRA_COMMON_HEADER_H
#define HIRA_COMMON_HEADER_H

#ifndef PREPROCESSING
#include <bits/stdc++.h>
#endif  // PREPROCESSING

#ifdef LOCAL

#define ASSERT(x)                                                          \
  ((x) || (fprintf(stderr, "assertion failed (" __FILE__ ":%d): \"%s\"\n", \
                   __LINE__, #x),                                          \
           assert(false), false))

#else

#define ASSERT(...)

#endif  // LOCAL

using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

#endif  // HIRA_COMMON_HEADER_H