#ifndef PREPROCESSING
#include <bits/stdc++.h>
#endif

#include "local/debug.h"
#include "local/platform.h"

using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using i128 = __int128_t;
using u128 = __uint128_t;

void Initialize();
void SolveCase(int Case);

int main(int argc, char* argv[]) {
  Initialize();

  int T = 1;
#ifdef MULTIPLE_TASK
  std::cin >> T;
#endif
  for (int t = 1; t <= T; ++t)
    SolveCase(t);
  return 0;
}
