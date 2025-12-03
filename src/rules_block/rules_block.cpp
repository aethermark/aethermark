// Copyright 2025 Aethermark Contributors
// All rights reserved.

#include "aethermark/rules_block/rules_block.hpp"

#include <string>
#include <vector>

#include "aethermark/aethermark.hpp"
#include "aethermark/parser_block.hpp"
#include "aethermark/utils.hpp"

namespace aethermark {

// NOLINTBEGIN(runtime/references)

// FIXME: Implement all block rules
bool rules_blockquote(StateBlock& state, int startLine, int endLine,
                      bool silent) {
  int pos = state.bMarks[startLine] + state.tShift[startLine];
  int max = state.eMarks[startLine];

  const int oldLineMax = state.lineMax;

  // If indented too far (more than 3 spaces) -> not a blockquote
  if (state.sCount[startLine] - state.blkIndent >= 4) return false;

  // Must start with '>'
  if (state.src[pos] != '>') return false;

  if (silent) return true;

  std::vector<int> oldBMarks;
  std::vector<int> oldBSCount;
  std::vector<int> oldSCount;
  std::vector<int> oldTShift;

  auto terminators = state.md.blockParser.ruler.getRules("blockquote");

  ParentType oldParent = state.parentType;
  state.parentType = ParentType::Blockquote;

  bool lastLineEmpty = false;
  int nextLine;

  // Search
  for (nextLine = startLine; nextLine < endLine; nextLine++) {
    bool isOutdented = state.sCount[nextLine] < state.blkIndent;

    pos = state.bMarks[nextLine] + state.tShift[nextLine];
    max = state.eMarks[nextLine];

    if (pos >= max) {
      // Empty line outside blockquote
      break;
    }

    // If the line starts with '>' and not outdented
    if (state.src[pos] == '>' && !isOutdented) {
      pos++;

      int initial = state.sCount[nextLine] + 1;
      bool spaceAfterMarker = false;
      bool adjustTab = false;

      // Optional space after >
      if (pos < max && state.src[pos] == ' ') {
        pos++;
        initial++;
        spaceAfterMarker = true;
      } else if (pos < max && state.src[pos] == '\t') {
        spaceAfterMarker = true;
        if ((state.bsCount[nextLine] + initial) % 4 == 3) {
          pos++;
          initial++;
        } else {
          adjustTab = true;
        }
      }

      int offset = initial;

      oldBMarks.push_back(state.bMarks[nextLine]);
      state.bMarks[nextLine] = pos;

      // Consume leading whitespace
      while (pos < max) {
        unsigned char ch = state.src[pos];
        if (isSpace(static_cast<int>(ch))) {
          if (ch == '\t') {
            offset +=
                4 -
                (offset + state.bsCount[nextLine] + (adjustTab ? 1 : 0)) % 4;
          } else {
            offset++;
          }
        } else {
          break;
        }
        pos++;
      }

      lastLineEmpty = (pos >= max);

      oldBSCount.push_back(state.bsCount[nextLine]);
      state.bsCount[nextLine] =
          state.sCount[nextLine] + 1 + (spaceAfterMarker ? 1 : 0);

      oldSCount.push_back(state.sCount[nextLine]);
      state.sCount[nextLine] = offset - initial;

      oldTShift.push_back(state.tShift[nextLine]);
      state.tShift[nextLine] = pos - state.bMarks[nextLine];

      continue;
    }

    // Case 2: last line was empty
    if (lastLineEmpty) break;

    // Case 3: terminator rule
    bool terminate = false;
    for (auto& rule : terminators) {
      if (rule(state, nextLine, endLine, true)) {
        terminate = true;
        break;
      }
    }

    if (terminate) {
      state.lineMax = nextLine;

      if (state.blkIndent != 0) {
        oldBMarks.push_back(state.bMarks[nextLine]);
        oldBSCount.push_back(state.bsCount[nextLine]);
        oldTShift.push_back(state.tShift[nextLine]);
        oldSCount.push_back(state.sCount[nextLine]);

        state.sCount[nextLine] -= state.blkIndent;
      }

      break;
    }

    oldBMarks.push_back(state.bMarks[nextLine]);
    oldBSCount.push_back(state.bsCount[nextLine]);
    oldTShift.push_back(state.tShift[nextLine]);
    oldSCount.push_back(state.sCount[nextLine]);

    state.sCount[nextLine] = -1;  // paragraph continuation
  }

  int oldIndent = state.blkIndent;
  state.blkIndent = 0;

  // Open token
  Token& open = state.push("blockquote_open", "blockquote", Nesting::OPENING);
  open.markup = ">";
  open.map = std::optional<std::pair<float, float>>(
      {static_cast<float>(startLine), 0});

  // Inner contents
  state.md.blockParser.tokenize(state, startLine, nextLine);

  // Close token
  Token& close = state.push("blockquote_close", "blockquote", Nesting::CLOSING);
  close.markup = ">";

  // Fix map end
  open.map->second = static_cast<float>(state.line);
  state.lineMax = oldLineMax;
  state.parentType = oldParent;
  state.blkIndent = oldIndent;

  // Restore offsets
  for (int i = 0; i < static_cast<int>(oldTShift.size()); i++) {
    int line = startLine + i;
    state.bMarks[line] = oldBMarks[i];
    state.tShift[line] = oldTShift[i];
    state.sCount[line] = oldSCount[i];
    state.bsCount[line] = oldBSCount[i];
  }

  return true;
}

bool rules_code(StateBlock& state, int startLine, int endLine, bool silent) {}

bool rules_fence(StateBlock& state, int startLine, int endLine, bool silent) {}

bool rules_heading(StateBlock& state, int startLine, int endLine, bool silent) {
}

bool rules_hr(StateBlock& state, int startLine, int endLine, bool silent) {}

bool rules_html_block(StateBlock& state, int startLine, int endLine,
                      bool silent) {}

bool rules_lheading(StateBlock& state, int startLine, int endLine,
                    bool silent) {}

bool rules_list(StateBlock& state, int startLine, int endLine, bool silent) {}

bool rules_paragraph(StateBlock& state, int startLine, int endLine,
                     bool silent) {
  // If this is an empty line -> not a paragraph
  if (state.isEmpty(startLine)) return false;

  auto terminatorRules = state.md.blockParser.ruler.getRules("paragraph");

  ParentType oldParentType = state.parentType;
  state.parentType = ParentType::Paragraph;

  int nextLine = startLine + 1;

  // Move line-by-line until we find a terminator
  for (; nextLine < endLine && !state.isEmpty(nextLine); nextLine++) {
    // Code-indented line after a paragraph = lazy continuation
    if (state.sCount[nextLine] - state.blkIndent > 3) continue;

    // Blockquote marker quirk (negative indent)
    if (state.sCount[nextLine] < 0) continue;

    // Run terminator rules
    bool terminate = false;
    for (auto& rule : terminatorRules) {
      if (rule(state, nextLine, endLine, true)) {
        terminate = true;
        break;
      }
    }
    if (terminate) break;
  }

  // Extract raw paragraph text
  std::string content =
      state.getLines(startLine, nextLine, state.blkIndent, false);
  content = trim(content);  // your string helper

  state.line = nextLine;

  // Build tokens
  Token& token_open = state.push("paragraph_open", "p", Nesting::OPENING);
  token_open.map =
      std::optional<std::pair<float, float>>({startLine, state.line});

  Token& token_inline = state.push("inline", "", Nesting::SELF_CLOSING);
  token_inline.content = content;
  token_inline.map =
      std::optional<std::pair<float, float>>({startLine, state.line});

  state.push("paragraph_close", "p", Nesting::CLOSING);

  state.parentType = oldParentType;

  return true;
}

bool rules_reference(StateBlock& state, int startLine, int endLine,
                     bool silent) {}

bool rules_table(StateBlock& state, int startLine, int endLine, bool silent) {}

// NOLINTEND

}  // namespace aethermark
