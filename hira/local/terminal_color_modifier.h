#ifndef HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H
#define HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H

#include "hira/common_header.h"

#ifdef LOCAL

namespace TerminalColor {

enum ColorCode {
  FG_RED = 31,
  FG_GREEN = 32,
  FG_BLUE = 34,
  FG_DEFAULT = 39,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_BLUE = 44,
  BG_DEFAULT = 49
};

class ColorModifier {
  ColorCode color_code_;

 public:
  ColorModifier(ColorCode color_code) : color_code_(color_code) {}
  friend std::ostream& operator<<(std::ostream& os,
                                  const ColorModifier& modifier) {
    return os << "\033[" << modifier.color_code_ << "m";
  }
};

}  // namespace TerminalColor

#endif  // LOCAL

#endif  // HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H