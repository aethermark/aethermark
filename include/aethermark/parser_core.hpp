// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/ruler.hpp"
#include "aethermark/rules_core/state_core.hpp"

namespace aethermark {

/// @brief Alias for rule functions of the `ParserCore`.
using RuleCore = std::function<void(StateCore&)>;

/// @brief Parser for the core parsing stage.
class ParserCore {
 public:
  ParserCore();

  /// @brief Ruler instance holding core rules.
  Ruler<RuleCore> ruler;

  /// @brief Execute the core rule chain.
  /// @param state State for the parser.
  void Process(StateCore& state);  // NOLINT(runtime/references)
};

}  // namespace aethermark
