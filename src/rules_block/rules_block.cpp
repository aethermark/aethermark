// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_block/rules_block.hpp"

#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/parser_block.hpp"
#include "aethermark/token.hpp"
#include "aethermark/utils.hpp"

namespace aethermark {

// NOLINTBEGIN(runtime/references)

// FIXME: Implement all block rules
bool BlockRules::RuleBlockquote(StateBlock& state, int start_line, int end_line,
                                bool silent) {
  int pos = state.b_marks[start_line] + state.t_shift[start_line];
  int max = state.e_marks[start_line];

  const int oldline_max = state.line_max;

  // If indented too far (more than 3 spaces) -> not a blockquote
  if (state.s_count[start_line] - state.blk_indent >= 4) return false;

  // Must start with '>'
  if (state.src[pos] != '>') return false;

  if (silent) return true;

  std::vector<int> old_b_marks;
  std::vector<int> old_bs_count;
  std::vector<int> old_s_count;
  std::vector<int> old_t_shift;

  std::vector<std::pair<std::string, RuleBlock>> terminators =
      state.md.block_parser.ruler.GetRules("blockquote");

  ParentType old_parent = state.parent_type;
  state.parent_type = ParentType::kBlockquote;

  bool last_line_empty = false;
  int next_line;

  // Search
  for (next_line = start_line; next_line < end_line; next_line++) {
    bool is_outdented = state.s_count[next_line] < state.blk_indent;

    pos = state.b_marks[next_line] + state.t_shift[next_line];
    max = state.e_marks[next_line];

    if (pos >= max) {
      // Empty line outside blockquote
      break;
    }

    // If the line starts with '>' and not outdented
    if (state.src[pos] == '>' && !is_outdented) {
      pos++;

      int initial = state.s_count[next_line] + 1;
      bool space_after_marker = false;
      bool adjust_tab = false;

      // Optional space after >
      if (pos < max && state.src[pos] == ' ') {
        pos++;
        initial++;
        space_after_marker = true;
      } else if (pos < max && state.src[pos] == '\t') {
        space_after_marker = true;
        if ((state.bs_count[next_line] + initial) % 4 == 3) {
          pos++;
          initial++;
        } else {
          adjust_tab = true;
        }
      }

      int offset = initial;

      old_b_marks.push_back(state.b_marks[next_line]);
      state.b_marks[next_line] = pos;

      // Consume leading whitespace
      while (pos < max) {
        unsigned char ch = state.src[pos];
        if (Utils::IsSpace(static_cast<int>(ch))) {
          if (ch == '\t') {
            offset +=
                4 -
                (offset + state.bs_count[next_line] + (adjust_tab ? 1 : 0)) % 4;
          } else {
            offset++;
          }
        } else {
          break;
        }
        pos++;
      }

      last_line_empty = (pos >= max);

      old_bs_count.push_back(state.bs_count[next_line]);
      state.bs_count[next_line] =
          state.s_count[next_line] + 1 + (space_after_marker ? 1 : 0);

      old_s_count.push_back(state.s_count[next_line]);
      state.s_count[next_line] = offset - initial;

      old_t_shift.push_back(state.t_shift[next_line]);
      state.t_shift[next_line] = pos - state.b_marks[next_line];

      continue;
    }

    // Case 2: last line was empty
    if (last_line_empty) break;

    // Case 3: terminator rule
    bool terminate = false;
    for (std::pair<std::string, RuleBlock>& rule : terminators) {
      if (rule.second(state, next_line, end_line, true)) {
        terminate = true;
        break;
      }
    }

    if (terminate) {
      state.line_max = next_line;

      if (state.blk_indent != 0) {
        old_b_marks.push_back(state.b_marks[next_line]);
        old_bs_count.push_back(state.bs_count[next_line]);
        old_t_shift.push_back(state.t_shift[next_line]);
        old_s_count.push_back(state.s_count[next_line]);

        state.s_count[next_line] -= state.blk_indent;
      }

      break;
    }

    old_b_marks.push_back(state.b_marks[next_line]);
    old_bs_count.push_back(state.bs_count[next_line]);
    old_t_shift.push_back(state.t_shift[next_line]);
    old_s_count.push_back(state.s_count[next_line]);

    state.s_count[next_line] = -1;  // paragraph continuation
  }

  int old_indent = state.blk_indent;
  state.blk_indent = 0;

  // Open token
  Token& open = state.Push("blockquote_open", "blockquote", Nesting::kOpening);
  open.markup = ">";
  open.map = std::optional<std::pair<float, float>>(
      {static_cast<float>(start_line), 0});

  // Inner contents
  state.md.block_parser.Tokenize(state, start_line, next_line);

  // Close token
  Token& close =
      state.Push("blockquote_close", "blockquote", Nesting::kClosing);
  close.markup = ">";

  // Fix map end
  open.map->second = static_cast<float>(state.line);
  state.line_max = oldline_max;
  state.parent_type = old_parent;
  state.blk_indent = old_indent;

  // Restore offsets
  for (int i = 0; i < static_cast<int>(old_t_shift.size()); i++) {
    int line = start_line + i;
    state.b_marks[line] = old_b_marks[i];
    state.t_shift[line] = old_t_shift[i];
    state.s_count[line] = old_s_count[i];
    state.bs_count[line] = old_bs_count[i];
  }

  return true;
}

bool BlockRules::RuleCode(StateBlock& state, int start_line, int end_line,
                          bool silent) {
  if (state.s_count[start_line] - state.blk_indent < 4) {
    return false;
  }

  int next_line = start_line + 1;
  int last = next_line;

  // Find end of block
  while (next_line < end_line) {
    if (state.IsEmpty(next_line)) {
      next_line++;
      continue;
    }
    if (state.s_count[next_line] - state.blk_indent >= 4) {
      next_line++;
      last = next_line;
      continue;
    }
    break;
  }

  state.line = last;

  Token& token = state.Push("code_block", "code", Nesting::kSelfClosing);
  token.content =
      state.GetLines(start_line, last, 4 + state.blk_indent, false) + "\n";
  token.map = {start_line, state.line};

  return true;
}

bool BlockRules::RuleFence(StateBlock& state, int start_line, int end_line,
                           bool silent) {
  int pos = state.b_marks[start_line] + state.t_shift[start_line];
  int max = state.e_marks[start_line];

  // if it's indented more than 3 spaces, it should be a code block
  if (state.s_count[start_line] - state.blk_indent >= 4) {
    return false;
  }

  if (pos + 3 > max) return false;

  const char marker = state.src[pos];

  if (marker != 0x7E /* ~ */ && marker != 0x60 /* ` */) {
    return false;
  }

  // scan marker length
  int mem = pos;
  pos = state.SkipChars(pos, marker);

  int len = pos - mem;

  if (len < 3) {
    return false;
  }

  std::string markup = Utils::Slice(state.src, mem, pos);
  std::string params = Utils::Slice(state.src, pos, max);

  if (marker == 0x60 /* ` */) {
    if (params.find(marker) != std::string::npos) {
      return false;
    }
  }

  // Since start is found, we can report success here in validation mode
  if (silent) {
    return true;
  }

  // search end of block
  int next_line = start_line;
  int have_end_marker = false;

  while (true) {
    next_line++;

    if (next_line >= end_line) {
      // unclosed block should be autoclosed by end of document.
      // also block seems to be autoclosed by end of parent
      break;
    }

    pos = mem = state.b_marks[next_line] + state.t_shift[next_line];
    max = state.e_marks[next_line];

    if (pos < max && state.s_count[next_line] < state.blk_indent) {
      // non-empty line with negative indent should stop the list:
      // - ```
      //  test
      break;
    }

    if (state.src[pos] != marker) continue;

    if (state.s_count[next_line] - state.blk_indent >= 4) {
      // closing fence should be indented less than 4 spaces
      continue;
    }

    pos = state.SkipChars(pos, marker);

    // closing code fence must be at least as long as the opening one
    if (pos - mem < len) continue;

    // make sure tail has spaces only
    pos = state.SkipSpaces(pos);

    if (pos < max) continue;

    // found!
    have_end_marker = true;
    break;
  }

  // If a fence has heading spaces, they should be removed from its inner block
  len = state.s_count[start_line];

  state.line = next_line + (have_end_marker ? 1 : 0);

  Token& token = state.Push("fence", "code", Nesting::kSelfClosing);
  token.info = params;
  token.content = state.GetLines(start_line + 1, next_line, len, true);
  token.markup = markup;
  token.map = {start_line, state.line};

  return true;
}

bool BlockRules::RuleHeading(StateBlock& state, int start_line, int end_line,
                             bool silent) {
  return false;
}

bool BlockRules::RuleHr(StateBlock& state, int start_line, int end_line,
                        bool silent) {
  return false;
}

bool BlockRules::RuleHtmlBlock(StateBlock& state, int start_line, int end_line,
                               bool silent) {
  return false;
}

bool BlockRules::RuleLheading(StateBlock& state, int start_line, int end_line,
                              bool silent) {
  return false;
}

bool BlockRules::RuleList(StateBlock& state, int start_line, int end_line,
                          bool silent) {
  return false;
}

bool BlockRules::RuleParagraph(StateBlock& state, int start_line, int end_line,
                               bool silent) {
  // If this is an empty line -> not a paragraph
  if (state.IsEmpty(start_line)) return false;

  std::vector<std::pair<std::string, RuleBlock>> terminator_rules =
      state.md.block_parser.ruler.GetRules("paragraph");

  ParentType old_parent_type = state.parent_type;
  state.parent_type = ParentType::kParagraph;

  int next_line = start_line + 1;

  // Move line-by-line until we find a terminator
  for (; next_line < end_line && !state.IsEmpty(next_line); next_line++) {
    // Code-indented line after a paragraph = lazy continuation
    if (state.s_count[next_line] - state.blk_indent > 3) continue;

    // Blockquote marker quirk (negative indent)
    if (state.s_count[next_line] < 0) continue;

    // Run terminator rules
    bool terminate = false;
    for (std::pair<std::string, RuleBlock>& rule : terminator_rules) {
      if (rule.second(state, next_line, end_line, true)) {
        terminate = true;
        break;
      }
    }
    if (terminate) break;
  }

  // Extract raw paragraph text
  std::string content =
      state.GetLines(start_line, next_line, state.blk_indent, false);
  content = Utils::Trim(content);

  state.line = next_line;

  // Build tokens
  Token& token_open = state.Push("paragraph_open", "p", Nesting::kOpening);
  token_open.map =
      std::optional<std::pair<float, float>>({start_line, state.line});

  Token& token_inline = state.Push("inline", "", Nesting::kSelfClosing);
  token_inline.content = content;
  token_inline.map =
      std::optional<std::pair<float, float>>({start_line, state.line});

  state.Push("paragraph_close", "p", Nesting::kClosing);

  state.parent_type = old_parent_type;

  return true;
}

bool BlockRules::RuleReference(StateBlock& state, int start_line, int end_line,
                               bool silent) {
  return false;
}

bool BlockRules::RuleTable(StateBlock& state, int start_line, int end_line,
                           bool silent) {
  return false;
}

// NOLINTEND

}  // namespace aethermark
