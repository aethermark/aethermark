// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/ruler.hpp"
#include "aethermark/rules_core/state_core.hpp"

namespace aethermark {

using RuleCore = std::function<void(StateCore&)>;
using State = StateCore;

class Core {
 public:
  Core();

  /// @brief Ruler instance holding core rules.
  Ruler<RuleCore> ruler;

  /// @brief Execute the core rule chain.
  /// @param state State for the parser.
  void process(StateCore& state);  // NOLINT(runtime/references)
};

}  // namespace aethermark
