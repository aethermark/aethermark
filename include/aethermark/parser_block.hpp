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

using RuleBlock = std::function<bool(StateBlock&, int, int, bool)>;

class ParserBlock {
 public:
  ParserBlock();

  /// @brief Ruler with block-level rules.
  Ruler<RuleBlock> ruler;

  /// @brief Generate tokens for input range.
  /// @param state Reference to the parser state.
  /// @param startLine Index of the starting line.
  /// @param endLine Index of the ending line.
  void tokenize(StateBlock& state,  // NOLINT(runtime/references)
                int startLine, int endLine);

  // NOLINTBEGIN(runtime/references)

  /// @brief Process the input string fully and push tokens into outTokens.
  /// @param str Source text.
  /// @param md Reference to the parser.
  /// @param env Environment for parser.
  /// @param outTokens Token output list.
  void parse(const std::string& str, Aethermark& md, std::any env,
             std::deque<Token>& outTokens);
  // NOLINTEND

  using State = StateBlock;
};

}  // namespace aethermark
