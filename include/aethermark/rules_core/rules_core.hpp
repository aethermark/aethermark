// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_core/state_core.hpp"

namespace aethermark {

/// @brief Static class for all the core parser rules.
class CoreRules {
 public:
  CoreRules() = delete;

  static void RuleBlock(StateCore& state);  // NOLINT(runtime/references)

  static void RuleInline(StateCore& state);  // NOLINT(runtime/references)

  static void RuleLinkify(StateCore& state);  // NOLINT(runtime/references)

  static void RuleNormalize(StateCore& state);  // NOLINT(runtime/references)

  static void RuleReplace(StateCore& state);  // NOLINT(runtime/references)

  static void RuleSmartquotes(StateCore& state);  // NOLINT(runtime/references)

  static void RuleTextJoin(StateCore& state);  // NOLINT(runtime/references)
};

}  // namespace aethermark
