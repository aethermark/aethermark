// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_core/state_core.hpp"

namespace aethermark {

void rule_block(StateCore& state);  // NOLINT(runtime/references)

void rule_inline(StateCore& state);  // NOLINT(runtime/references)

void rule_linkify(StateCore& state);  // NOLINT(runtime/references)

void rule_normalize(StateCore& state);  // NOLINT(runtime/references)

void rule_replace(StateCore& state);  // NOLINT(runtime/references)

void rule_smartquotes(StateCore& state);  // NOLINT(runtime/references)

void rule_text_join(StateCore& state);  // NOLINT(runtime/references)

}  // namespace aethermark
