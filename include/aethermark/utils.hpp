// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <string>

namespace aethermark {

class StateBlock;

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

  /// @brief Returns a substring sliced between the given indices.
  /// @param s Input string.
  /// @param start Start index (inclusive).
  /// @param end End index (exclusive).
  /// @return The sliced substring.
  static std::string Slice(const std::string& s, int start, int end);

  /// @brief Search `[-+*][\n ]`.
  /// @param state State of parser.
  /// @param start_line Initial position.
  /// @return Next pos after marker on success or -1 on fail.
  static int SkipBulletListMarker(const StateBlock& state, int start_line);

  /// @brief Search `\d+[.)][\n ]`.
  /// @param state State of parser.
  /// @param start_line Initial position.
  /// @return returns Next pos after marker on success or -1 on fail.
  static int SkipOrderedListmarker(const StateBlock& state, int start_line);

  /// @brief Marks tight paragraphs inside a list starting from the given line.
  /// @param state State of parser.
  /// @param idx Starting line index.
  static void MarkTightParagraphs(const StateBlock& state, int idx);
};

}  // namespace aethermark
