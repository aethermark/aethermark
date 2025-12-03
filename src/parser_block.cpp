// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/parser_block.hpp"

#include <deque>
#include <string>
#include <tuple>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/ruler.hpp"
#include "aethermark/rules_block/rules_block.hpp"
#include "aethermark/rules_block/state_block.hpp"
#include "aethermark/token.hpp"

namespace aethermark {

static const std::vector<
    std::tuple<std::string, RuleBlock, std::vector<std::string>>>
    block_rules = {{"table", BlockRules::RuleTable, {"paragraph", "reference"}},
                   {"code", BlockRules::RuleCode, {}},
                   {"fence",
                    BlockRules::RuleFence,
                    {"paragraph", "reference", "blockquote", "list"}},
                   {"blockquote",
                    BlockRules::RuleBlockquote,
                    {"paragraph", "reference", "blockquote", "list"}},
                   {"hr",
                    BlockRules::RuleHr,
                    {"paragraph", "reference", "blockquote", "list"}},
                   {"list",
                    BlockRules::RuleList,
                    {"paragraph", "reference", "blockquote"}},
                   {"reference", BlockRules::RuleReference, {}},
                   {"html_block",
                    BlockRules::RuleHtmlBlock,
                    {"paragraph", "reference", "blockquote"}},
                   {"heading",
                    BlockRules::RuleHeading,
                    {"paragraph", "reference", "blockquote"}},
                   {"lheading", BlockRules::RuleLheading, {}},
                   {"paragraph", BlockRules::RuleParagraph, {}}};

ParserBlock::ParserBlock() : ruler() {
  for (const auto& [name, fn, alts] : block_rules) {
    RuleOptions opts;
    opts.alt = alts;
    ruler.Push(name, fn, opts);
  }
}

void ParserBlock::Tokenize(StateBlock& state, int startLine, int endLine) {
  // Get rule list
  const auto& rules = ruler.GetRules("");
  const int len = static_cast<int>(rules.size());
  const int maxNesting = state.md.options.max_nesting;

  int line = startLine;
  bool hasEmptyLines = false;

  while (line < endLine) {
    // Skip empty lines
    state.line = state.SkipEmptyLines(line);
    line = state.line;
    if (line >= endLine) break;

    // Termination for nested blocks (quotes, lists, etc.)
    if (state.s_count[line] < state.blk_indent) {
      break;
    }

    // Max nesting level reached → bail out
    if (state.level >= maxNesting) {
      state.line = endLine;
      break;
    }

    const int prevLine = state.line;
    bool matched = false;

    // Try all block rules
    for (int i = 0; i < len; ++i) {
      const auto& rule = rules[i];
      if (rule(state, line, endLine, false)) {
        matched = true;
        if (state.line <= prevLine) {
          throw std::runtime_error("block rule didn't increment state.line");
        }
        break;
      }
    }

    // Paragraph rule disabled? → impossible in normal config
    if (!matched) {
      throw std::runtime_error("none of the block rules matched");
    }

    // Update state.tight (same logic as JS)
    state.tight = !hasEmptyLines;

    // Paragraphs sometimes consume the trailing newline
    if (state.IsEmpty(state.line - 1)) {
      hasEmptyLines = true;
    }

    line = state.line;

    // If next line is empty, mark it
    if (line < endLine && state.IsEmpty(line)) {
      hasEmptyLines = true;
      ++line;
      state.line = line;
    }
  }
}

void ParserBlock::Parse(const std::string& src, Aethermark& md, std::any env,
                        std::deque<Token>& outTokens) {
  if (src.empty()) {
    return;
  }

  // Create parsing state
  StateBlock state(src, md, env, outTokens);

  // Kick off block tokenization
  Tokenize(state, state.line, state.line_max);
}

}  // namespace aethermark
