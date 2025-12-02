// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_block/state_block.hpp"

namespace aethermark {

// NOLINTBEGIN(runtime/references)

bool rules_blockquote(StateBlock& state, int startLine, int endLine,
                      bool silent);  // NOLINT(whitespace/indent_namespace)

bool rules_code(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_fence(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_heading(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_hr(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_html_block(StateBlock& state, int startLine, int endLine,
                      bool silent);  // NOLINT(whitespace/indent_namespace)

bool rules_lheading(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_list(StateBlock& state, int startLine, int endLine, bool silent);

bool rules_paragraph(StateBlock& state, int startLine, int endLine,
                     bool silent);  // NOLINT(whitespace/indent_namespace)

bool rules_reference(StateBlock& state, int startLine, int endLine,
                     bool silent);  // NOLINT(whitespace/indent_namespace)

bool rules_table(StateBlock& state, int startLine, int endLine, bool silent);

// NOLINTEND

}  // namespace aethermark
