#include "hira/common_header.h"

#include "hira/local/debug.h"
#include "hira/local/platform.h"
#include "hira/local/time_calculator.h"

#include "hira/misc/io.h"

using namespace hira;

void Initialize();
void SolveCase(int Case);

int main(int argc, char* argv[]) {
  STDIO;

  Initialize();

  int T = 1;
#ifdef MULTIPLE_TASK
  std::cin >> T;
#endif
  for (int t = 1; t <= T; ++t)
    SolveCase(t);
  return 0;
}
