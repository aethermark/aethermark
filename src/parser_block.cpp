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

// NOLINTBEGIN(whitespace/indent_namespace)

static const std::vector<
    std::tuple<std::string, RuleBlock, std::vector<std::string>>>
    block_rules = {
        {"table", rules_table, {"paragraph", "reference"}},
        {"code", rules_code, {}},
        {"fence",
         rules_fence,
         {"paragraph", "reference", "blockquote", "list"}},
        {"blockquote",
         rules_blockquote,
         {"paragraph", "reference", "blockquote", "list"}},
        {"hr", rules_hr, {"paragraph", "reference", "blockquote", "list"}},
        {"list", rules_list, {"paragraph", "reference", "blockquote"}},
        {"reference", rules_reference, {}},
        {"html_block",
         rules_html_block,
         {"paragraph", "reference", "blockquote"}},
        {"heading", rules_heading, {"paragraph", "reference", "blockquote"}},
        {"lheading", rules_lheading, {}},
        {"paragraph", rules_paragraph, {}}};
// NOLINTEND

ParserBlock::ParserBlock() : ruler() {
  for (const auto& [name, fn, alts] : block_rules) {
    RuleOptions opts;
    opts.alt = alts;
    ruler.push(name, fn, opts);
  }
}

void ParserBlock::tokenize(StateBlock& state, int startLine, int endLine) {
  // Get rule list
  const auto& rules = ruler.getRules("");
  const int len = static_cast<int>(rules.size());
  const int maxNesting = state.md.options.maxNesting;

  int line = startLine;
  bool hasEmptyLines = false;

  while (line < endLine) {
    // Skip empty lines
    state.line = state.skipEmptyLines(line);
    line = state.line;
    if (line >= endLine) break;

    // Termination for nested blocks (quotes, lists, etc.)
    if (state.sCount[line] < state.blkIndent) {
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
    if (state.isEmpty(state.line - 1)) {
      hasEmptyLines = true;
    }

    line = state.line;

    // If next line is empty, mark it
    if (line < endLine && state.isEmpty(line)) {
      hasEmptyLines = true;
      ++line;
      state.line = line;
    }
  }
}

// NOLINTBEGIN(whitespace/indent_namespace)

void ParserBlock::parse(const std::string& src, Aethermark& md, std::any env,
                        std::deque<Token>& outTokens) {
  // NOLINTEND

  if (src.empty()) {
    return;
  }

  // Create parsing state
  StateBlock state(src, md, env, outTokens);

  // Kick off block tokenization
  tokenize(state, state.line, state.lineMax);
}

}  // namespace aethermark
