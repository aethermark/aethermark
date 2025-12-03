// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include <any>
#include <deque>
#include <string>
#include <vector>

#include "aethermark/token.hpp"

namespace aethermark {
class Aethermark;

class StateCore {
 public:
  /// @brief Constructs a new core state object.
  /// @param src Document source.
  /// @param md Reference to the parser.
  /// @param env Environment for parser.
  StateCore(const std::string& src,
            Aethermark& md,  // NOLINT(runtime/references)
            std::any env);

  /// @brief Source string being parsed.
  std::string src;

  /// @brief Environment data.
  std::any env;

  /// @brief List of tokens generated during parsing.
  std::deque<Token> tokens;

  /// @brief Whether the state is for inline mode.
  bool inlineMode = false;

  /// @brief Reference to the main Aethermark instance.
  Aethermark& md;  // NOLINT(runtime/references)

  using TokenType = Token;
};

}  // namespace aethermark
