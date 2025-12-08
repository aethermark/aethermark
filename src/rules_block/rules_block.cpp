// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_block/rules_block.hpp"

#include <algorithm>
#include <string>
#include <utility>
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
  int pos = state.b_marks[start_line] + state.t_shift[start_line];
  int max = state.e_marks[start_line];

  // if it's indented more than 3 spaces, it should be a code block
  if (state.s_count[start_line] - state.blk_indent >= 4) {
    return false;
  }

  char ch = state.src[pos];

  if (ch != 0x23 /* # */ || pos >= max) {
    return false;
  }

  // count heading level
  int level = 1;

  ch = state.src[++pos];
  while (ch == 0x23 /* # */ && pos < max && level <= 6) {
    level++;
    ch = state.src[++pos];
  }

  if (level > 6 || (pos < max && !Utils::IsSpace(ch))) {
    return false;
  }

  if (silent) {
    return true;
  }

  // Let's cut tails like '    ###  ' from the end of string

  max = state.SkipSpacesBack(max, pos);
  int tmp = state.SkipCharsBack(max, 0x23, pos);  // #
  if (tmp > pos && Utils::IsSpace(state.src[tmp - 1])) {
    max = tmp;
  }

  state.line = start_line + 1;

  Token& token_o = state.Push("heading_open", "h" + std::to_string(level),
                              Nesting::kOpening);
  std::string header_scafolding = "########";
  token_o.markup = Utils::Slice(header_scafolding, 0, level);
  token_o.map = {start_line, state.line};

  Token& token_i = state.Push("inline", "", Nesting::kSelfClosing);
  token_i.content = Utils::Trim(Utils::Slice(state.src, pos, max));
  token_i.map = {start_line, state.line};
  token_i.children = {};

  Token& token_c = state.Push("heading_close", "h" + std::to_string(level),
                              Nesting::kClosing);
  token_c.markup = Utils::Slice(header_scafolding, 0, level);

  return true;
}

bool BlockRules::RuleHr(StateBlock& state, int start_line, int end_line,
                        bool silent) {
  int max = state.e_marks[start_line];

  // if it's indented more than 3 spaces, it should be a code block
  if (state.s_count[start_line] - state.blk_indent >= 4) {
    return false;
  }

  int pos = state.b_marks[start_line] + state.t_shift[start_line];
  const char marker = state.src[pos++];

  // Check hr marker
  if (marker != 0x2A /* * */ && marker != 0x2D /* - */ &&
      marker != 0x5F /* _ */) {
    return false;
  }

  // markers can be mixed with spaces, but there should be at least 3 of them

  int cnt = 1;
  while (pos < max) {
    const char ch = state.src[pos++];
    if (ch != marker && !Utils::IsSpace(ch)) {
      return false;
    }
    if (ch == marker) {
      cnt++;
    }
  }

  if (cnt < 3) return false;

  if (silent) return true;

  state.line = start_line + 1;

  Token& token = state.Push("hr", "hr", Nesting::kSelfClosing);
  token.map = {start_line, state.line};

  std::string markup;
  markup.reserve(cnt);
  for (size_t i = 0; i < cnt; ++i) markup += marker;

  token.markup = markup;

  return true;
}

bool BlockRules::RuleHtmlBlock(StateBlock& state, int start_line, int end_line,
                               bool silent) {
  return false;
}

bool BlockRules::RuleLheading(StateBlock& state, int start_line, int end_line,
                              bool silent) {
  std::vector<std::pair<std::string, aethermark::RuleBlock>> terminator_rules =
      state.md.block_parser.ruler.GetRules("paragraph");

  // if it's indented more than 3 spaces, it should be a code block
  if (state.s_count[start_line] - state.blk_indent >= 4) {
    return false;
  }

  ParentType old_parent_type = state.parent_type;
  state.parent_type =
      ParentType::kParagraph;  // use paragraph to match terminatorRules

  // jump line-by-line until empty one or EOF
  int level = 0;
  char marker;
  int next_line = start_line + 1;

  for (; next_line < end_line && !state.IsEmpty(next_line); next_line++) {
    // this would be a code block normally, but after paragraph
    // it's considered a lazy continuation regardless of what's there
    if (state.s_count[next_line] - state.blk_indent > 3) {
      continue;
    }

    // Check for underline in setext header
    if (state.s_count[next_line] >= state.blk_indent) {
      int pos = state.b_marks[next_line] + state.t_shift[next_line];
      const int max = state.e_marks[next_line];

      if (pos < max) {
        marker = state.src[pos];

        if (marker == 0x2D /* - */ || marker == 0x3D /* = */) {
          pos = state.SkipChars(pos, marker);
          pos = state.SkipSpaces(pos);

          if (pos >= max) {
            level = (marker == 0x3D /* = */ ? 1 : 2);
            break;
          }
        }
      }
    }

    // quirk for blockquotes, this line should already be checked by that rule
    if (state.s_count[next_line] < 0) {
      continue;
    }

    // Some tags can terminate paragraph without empty line.
    bool terminate = false;
    for (int i = 0, l = terminator_rules.size(); i < l; i++) {
      if (terminator_rules[i].second(state, next_line, end_line, true)) {
        terminate = true;
        break;
      }
    }
    if (terminate) break;
  }

  if (!level) {
    // Didn't find valid underline
    return false;
  }

  const std::string content = Utils::Trim(
      state.GetLines(start_line, next_line, state.blk_indent, false));

  state.line = next_line + 1;

  Token& token_o = state.Push("heading_open", "h" + std::to_string(level),
                              Nesting::kOpening);
  token_o.markup = marker;
  token_o.map = {start_line, state.line};

  Token& token_i = state.Push("inline", "", Nesting::kSelfClosing);
  token_i.content = content;
  token_i.map = {start_line, state.line - 1};
  token_i.children = {};

  Token& token_c = state.Push("heading_close", "h" + std::to_string(level),
                              Nesting::kClosing);
  token_c.markup = marker;

  state.parent_type = old_parent_type;

  return true;
}

bool BlockRules::RuleList(StateBlock& state, int start_line, int end_line,
                          bool silent) {
  int max, pos, start;
  int next_line = start_line;
  bool tight = true;

  // if it's indented more than 3 spaces, it should be a code block.
  if (state.s_count[next_line] - state.blk_indent >= 4) {
    return false;
  }

  // Special case:
  //  - item 1
  //   - item 2
  //    - item 3
  //     - item 4
  //      - this one is a paragraph continuation
  if (state.list_indent >= 0 &&
      state.s_count[next_line] - state.list_indent >= 4 &&
      state.s_count[next_line] < state.blk_indent) {
    return false;
  }

  bool is_terminating_paragraph = false;

  // limit conditions when list can interrupt
  // a paragraph (validation mode only)
  if (silent && state.parent_type == ParentType::kParagraph) {
    // Next list item should still terminate previous list item;
    if (state.s_count[next_line] >= state.blk_indent) {
      is_terminating_paragraph = true;
    }
  }

  // Detect list type and position after marker
  bool is_ordered;
  int marker_value;
  int pos_after_marker;

  if ((pos_after_marker = Utils::SkipOrderedListmarker(state, next_line)) >=
      0) {
    is_ordered = true;
    start = state.b_marks[next_line] + state.t_shift[next_line];
    marker_value =
        std::stoi(Utils::Slice(state.src, start, pos_after_marker - 1));

    // If we're starting a new ordered list right after
    // a paragraph, it should start with 1.
    if (is_terminating_paragraph && marker_value != 1) return false;
  } else if ((pos_after_marker =
                  Utils::SkipBulletListMarker(state, next_line)) >= 0) {
    is_ordered = false;
  } else {
    return false;
  }

  // If we're starting a new unordered list right after
  // a paragraph, first line should not be empty.
  if (is_terminating_paragraph) {
    if (state.SkipSpaces(pos_after_marker) >= state.e_marks[next_line])
      return false;
  }

  // For validation mode we can terminate immediately
  if (silent) {
    return true;
  }

  // We should terminate list on style change. Remember first one to compare.
  const char marker_char_code = state.src[pos_after_marker - 1];

  // Start list
  const int list_tok_idx = state.tokens.size();

  Token token;

  if (is_ordered) {
    token = state.Push("ordered_list_open", "ol", Nesting::kOpening);
    if (marker_value != 1) {
      if (!token.attrs) token.attrs.emplace();

      token.attrs->push_back({"start", std::to_string(marker_value)});
    }
  } else {
    token = state.Push("bullet_list_open", "ul", Nesting::kOpening);
  }

  std::pair<int, int> list_lines = {next_line, 0};
  token.map = list_lines;
  token.markup = marker_char_code;

  //
  // Iterate list items
  //

  bool prev_empty_end = false;
  const std::vector<std::pair<std::string, RuleBlock>> terminator_rules =
      state.md.block_parser.ruler.GetRules("list");

  const ParentType old_parent_type = state.parent_type;
  state.parent_type = ParentType::kList;

  while (next_line < end_line) {
    pos = pos_after_marker;
    max = state.e_marks[next_line];

    const int initial = state.s_count[next_line] + pos_after_marker -
                        (state.b_marks[next_line] + state.t_shift[next_line]);
    int offset = initial;

    while (pos < max) {
      const char ch = state.src[pos];

      if (ch == 0x09) {
        offset += 4 - (offset + state.bs_count[next_line]) % 4;
      } else if (ch == 0x20) {
        offset++;
      } else {
        break;
      }

      pos++;
    }

    const int content_start = pos;
    int indent_after_marker;

    if (content_start >= max) {
      // trimming space in "-    \n  3" case, indent is 1 here
      indent_after_marker = 1;
    } else {
      indent_after_marker = offset - initial;
    }

    // If we have more than 4 spaces, the indent is 1
    // (the rest is just indented code block)
    if (indent_after_marker > 4) {
      indent_after_marker = 1;
    }

    // "  -  test"
    //  ^^^^^ - calculating total length of this thing
    const int indent = initial + indent_after_marker;

    // Run subparser & write tokens
    token = state.Push("list_item_open", "li", Nesting::kOpening);
    token.markup = marker_char_code;
    std::pair<int, int> item_lines = {next_line, 0};
    token.map = item_lines;
    if (is_ordered) {
      token.info = Utils::Slice(state.src, start, pos_after_marker - 1);
    }

    // change current state, then restore it after parser subcall
    const bool old_tight = state.tight;
    const int old_t_shift = state.t_shift[next_line];
    const int old_s_count = state.s_count[next_line];

    //  - example list
    // ^ listIndent position will be here
    //   ^ blkIndent position will be here
    //
    const int old_list_indent = state.list_indent;
    state.list_indent = state.blk_indent;
    state.blk_indent = indent;

    state.tight = true;
    state.t_shift[next_line] = content_start - state.b_marks[next_line];
    state.s_count[next_line] = offset;

    if (content_start >= max && state.IsEmpty(next_line + 1)) {
      // workaround for this case
      // (list item is empty, list terminates before "foo"):
      // ~~~~~~~~
      //   -
      //
      //     foo
      // ~~~~~~~~
      state.line = std::min(state.line + 2, end_line);
    } else {
      state.md.block_parser.Tokenize(state, next_line, end_line);
    }

    // If any of list item is tight, mark list as tight
    if (!state.tight || prev_empty_end) {
      tight = false;
    }
    // Item become loose if finish with empty line,
    // but we should filter last element, because it means list finish
    prev_empty_end =
        (state.line - next_line) > 1 && state.IsEmpty(state.line - 1);

    state.blk_indent = state.list_indent;
    state.list_indent = old_list_indent;
    state.t_shift[next_line] = old_t_shift;
    state.s_count[next_line] = old_s_count;
    state.tight = old_tight;

    token = state.Push("list_item_close", "li", Nesting::kClosing);
    token.markup = marker_char_code;

    next_line = state.line;
    item_lines.second = next_line;

    if (next_line >= end_line) {
      break;
    }

    //
    // Try to check if list is terminated or continued.
    //
    if (state.s_count[next_line] < state.blk_indent) {
      break;
    }

    // if it's indented more than 3 spaces, it should be a code block
    if (state.s_count[next_line] - state.blk_indent >= 4) {
      break;
    }

    // fail if terminating block found
    bool terminate = false;
    for (int i = 0, l = terminator_rules.size(); i < l; i++) {
      if (terminator_rules[i].second(state, next_line, end_line, true)) {
        terminate = true;
        break;
      }
    }
    if (terminate) {
      break;
    }

    // fail if list has another type
    if (is_ordered) {
      pos_after_marker = Utils::SkipOrderedListmarker(state, next_line);
      if (pos_after_marker < 0) {
        break;
      }
      start = state.b_marks[next_line] + state.t_shift[next_line];
    } else {
      pos_after_marker = Utils::SkipBulletListMarker(state, next_line);
      if (pos_after_marker < 0) {
        break;
      }
    }

    if (marker_char_code != state.src[pos_after_marker - 1]) {
      break;
    }
  }

  // Finalize list
  if (is_ordered) {
    token = state.Push("ordered_list_close", "ol", Nesting::kClosing);
  } else {
    token = state.Push("bullet_list_close", "ul", Nesting::kClosing);
  }
  token.markup = marker_char_code;

  list_lines.second = next_line;
  state.line = next_line;

  state.parent_type = old_parent_type;

  // mark paragraphs tight if needed
  if (tight) {
    Utils::MarkTightParagraphs(state, list_tok_idx);
  }

  return true;
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
