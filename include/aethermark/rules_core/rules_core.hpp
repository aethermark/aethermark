// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_core/state_core.hpp"

namespace aethermark {

void RuleBlock(StateCore& state);  // NOLINT(runtime/references)

void RuleInline(StateCore& state);  // NOLINT(runtime/references)

void RuleLinkify(StateCore& state);  // NOLINT(runtime/references)

void RuleNormalize(StateCore& state);  // NOLINT(runtime/references)

void RuleReplace(StateCore& state);  // NOLINT(runtime/references)

void RuleSmartquotes(StateCore& state);  // NOLINT(runtime/references)

void RuleTextJoin(StateCore& state);  // NOLINT(runtime/references)

}  // namespace aethermark
