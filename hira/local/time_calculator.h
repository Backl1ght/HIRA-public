#ifndef HIRA_LOCAL_TIME_CALCULATOR_H
#define HIRA_LOCAL_TIME_CALCULATOR_H

#include "hira/common_header.h"

#ifdef LOCAL

#include "terminal_color_modifier.h"

class TimeCalculator {
 public:
  TimeCalculator() { begin_ = std::chrono::steady_clock::now(); }

  ~TimeCalculator() {
    end_ = std::chrono::steady_clock::now();

    auto duration = end_ - begin_;
    auto duration_milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    std::cerr << TerminalColor::ColorModifier(TerminalColor::FG_GREEN);
    std::cerr << "Time Elapsed: " << duration_milliseconds.count() << " ms"
              << std::endl;
    std::cerr << TerminalColor::ColorModifier(TerminalColor::FG_DEFAULT);
  }

 private:
  std::chrono::steady_clock::time_point begin_;
  std::chrono::steady_clock::time_point end_;
} _;

#endif  // LOCAL

#endif  // HIRA_LOCAL_TIME_CALCULATOR_H