#ifndef HIRA_LOCAL_TIME_CALCULATOR_H
#define HIRA_LOCAL_TIME_CALCULATOR_H

#include "hira/common_header.h"
#include "terminal_color_modifier.h"

class TimeCalculator {
 public:
  TimeCalculator() { begin_ = std::chrono::steady_clock::now(); }

  ~TimeCalculator() {
    end_ = std::chrono::steady_clock::now();

    auto duration = end_ - begin_;
    auto duration_milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    std::cerr << foreground_green_modifier;
    std::cerr << "Time Elapsed: " << duration_milliseconds.count() << " ms"
              << std::endl;
    std::cerr << foreground_default_modifier;
  }

 private:
  std::chrono::steady_clock::time_point begin_;
  std::chrono::steady_clock::time_point end_;
} _;

#endif  // HIRA_LOCAL_TIME_CALCULATOR_H