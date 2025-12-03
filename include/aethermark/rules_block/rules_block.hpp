// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_block/state_block.hpp"

namespace aethermark {

// NOLINTBEGIN(runtime/references)

bool RuleBlockquote(StateBlock& state, int start_line, int end_line,
                    bool silent);

bool RuleCode(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleFence(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleHeading(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleHr(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleHtmlBlock(StateBlock& state, int start_line, int end_line,
                   bool silent);

bool RuleLheading(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleList(StateBlock& state, int start_line, int end_line, bool silent);

bool RuleParagraph(StateBlock& state, int start_line, int end_line,
                   bool silent);

bool RuleReference(StateBlock& state, int start_line, int end_line,
                   bool silent);

bool RuleTable(StateBlock& state, int start_line, int end_line, bool silent);

// NOLINTEND

}  // namespace aethermark
