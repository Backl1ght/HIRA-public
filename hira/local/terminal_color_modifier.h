#ifndef HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H
#define HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H

#include "hira/common_header.h"

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

class TerminalColorModifier {
  ColorCode color_code_;

 public:
  TerminalColorModifier(ColorCode color_code) : color_code_(color_code) {}
  friend std::ostream& operator<<(std::ostream& os,
                                  const TerminalColorModifier& modifier) {
    return os << "\033[" << modifier.color_code_ << "m";
  }
};

}  // namespace TerminalColor

TerminalColor::TerminalColorModifier foreground_red_modifier(
    TerminalColor::FG_RED);
TerminalColor::TerminalColorModifier foreground_green_modifier(
    TerminalColor::FG_GREEN);
TerminalColor::TerminalColorModifier foreground_default_modifier(
    TerminalColor::FG_DEFAULT);

#endif  // HIRA_LOCAL_TERMINAL_COLOR_MODIFIER_H