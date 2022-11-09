// #define MULTIPLE_TASK
#include "hira/main.cpp"

void Initialize() {}

void SolveCase(int Case) {
  int a, b;
  std::cin >> a >> b;
  logd(a, b, a + b);

  std::cout << a + b << std::endl;
}
