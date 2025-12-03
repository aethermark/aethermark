// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_block/state_block.hpp"

namespace aethermark {

// NOLINTBEGIN(runtime/references)

bool rule_blockquote(StateBlock& state, int startLine, int endLine,
                     bool silent);

bool rule_code(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_fence(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_heading(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_hr(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_html_block(StateBlock& state, int startLine, int endLine,
                     bool silent);

bool rule_lheading(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_list(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_paragraph(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_reference(StateBlock& state, int startLine, int endLine, bool silent);

bool rule_table(StateBlock& state, int startLine, int endLine, bool silent);

// NOLINTEND

}  // namespace aethermark
