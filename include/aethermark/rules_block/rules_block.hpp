// Copyright 2025 Aethermark Contributors
// All rights reserved.

#pragma once

#include "aethermark/rules_block/state_block.hpp"

namespace aethermark {

/// @brief Static class for all the block parser rules.
class BlockRules {
 public:
  BlockRules() = delete;

  // NOLINTBEGIN(runtime/references)

  static bool RuleBlockquote(StateBlock& state, int start_line, int end_line,
                             bool silent);

  static bool RuleCode(StateBlock& state, int start_line, int end_line,
                       bool silent);

  static bool RuleFence(StateBlock& state, int start_line, int end_line,
                        bool silent);

  static bool RuleHeading(StateBlock& state, int start_line, int end_line,
                          bool silent);

  static bool RuleHr(StateBlock& state, int start_line, int end_line,
                     bool silent);

  static bool RuleHtmlBlock(StateBlock& state, int start_line, int end_line,
                            bool silent);

  static bool RuleLheading(StateBlock& state, int start_line, int end_line,
                           bool silent);

  static bool RuleList(StateBlock& state, int start_line, int end_line,
                       bool silent);

  static bool RuleParagraph(StateBlock& state, int start_line, int end_line,
                            bool silent);

  static bool RuleReference(StateBlock& state, int start_line, int end_line,
                            bool silent);

  static bool RuleTable(StateBlock& state, int start_line, int end_line,
                        bool silent);

  // NOLINTEND
};

}  // namespace aethermark
