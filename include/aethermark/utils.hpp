// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <string>

namespace aethermark {

/// @brief Utility class for miscellaneous methods
class Utils {
 public:
  Utils() = delete;

  /// @brief Remove leading and trailing whitespace.
  /// @param s The given string.
  /// @return String without leading or trailing whitespace.
  static std::string Trim(const std::string& s);

  /// @brief Whether a character is "space" or not.
  /// @param ch The given character
  /// @return `true` if "space".
  static bool IsSpace(int ch);

  static std::string Slice(const std::string& s, int start, int end);
};

}  // namespace aethermark
