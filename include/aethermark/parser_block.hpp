// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <deque>
#include <functional>
#include <string>
#include <vector>

// #include "aethermark/aethermark.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/rules_block/state_block.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

class Aethermark;

/// @brief Alias for rule functions of the `ParserBlock`.
using RuleBlock = std::function<bool(StateBlock&, int, int, bool)>;

/// @brief Parser for the block parsing stage.
class ParserBlock {
 public:
  ParserBlock();

  /// @brief Ruler with block-level rules.
  Ruler<RuleBlock> ruler;

  /// @brief Generate tokens for input range.
  /// @param state Reference to the parser state.
  /// @param start_line Index of the starting line.
  /// @param end_line Index of the ending line.
  void Tokenize(StateBlock& state,  // NOLINT(runtime/references)
                int start_line, int end_line);

  // NOLINTBEGIN(runtime/references)

  /// @brief Process the input string fully and push tokens into outTokens.
  /// @param str Source text.
  /// @param md Reference to the parser.
  /// @param env Environment for parser.
  /// @param out_tokens Token output list.
  void Parse(const std::string& str, Aethermark& md, std::any env,
             std::deque<Token>& out_tokens);
  // NOLINTEND
};

}  // namespace aethermark
